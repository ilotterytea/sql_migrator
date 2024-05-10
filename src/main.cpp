#include <algorithm>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <optional>
#include <pqxx/pqxx>
#include <string>
#include <vector>

#include "config.hpp"
#include "generator.hpp"
#include "init.hpp"
#include "util.hpp"

int main(int argc, char *argv[]) {
  std::cout << "hi!" << std::endl;

  std::vector<std::string> args(argv + 1, argv + argc);

  std::optional<migrator::Configuration> configuration =
      migrator::parse_configuration_from_cli(args);

  if (!configuration.has_value()) {
    return 1;
  }

  std::optional<migrator::Mode> mode = migrator::parse_mode(args);

  if (!mode.has_value()) {
    std::cerr << "Missing the mode argument!\n"
              << "Run with -h or --help argument to get more information\n";
    return 1;
  }

  if (configuration->db_name.empty() || configuration->db_user.empty() ||
      configuration->db_pass.empty() || configuration->db_host.empty() ||
      configuration->db_port.empty()) {
    std::cerr << "Database credentials must be set!\n"
              << "Run with -h or --help argument to get more information\n";

    return 1;
  }

  const std::string &folder_path = configuration->migration_dir;

  bool directory_exists = std::filesystem::exists(folder_path);
  bool is_directory = std::filesystem::is_directory(folder_path);

  if (directory_exists && !is_directory) {
    std::cout << "*** The given path \"" << folder_path
              << "\" isn't a directory!\n";
    return 1;
  }

  if (!directory_exists) {
    std::cout << "Creating the \"" << folder_path << "\" directory...\n";
    std::filesystem::create_directories(folder_path);
  }

  if (*mode == migrator::Mode::GENERATE) {
    migrator::generate_migration_folder(*configuration, args);
    return 0;
  }

  migrator::create_sqlm_table_migration(*configuration);

  // validating directories
  std::vector<std::string> valid_directory_paths;
  for (const auto &entry : std::filesystem::directory_iterator(folder_path)) {
    std::string path = entry.path();

    bool up_exists = std::filesystem::exists(path + "/up.sql");
    bool down_exists = std::filesystem::exists(path + "/down.sql");

    if (up_exists && down_exists) {
      valid_directory_paths.push_back(path);
    } else {
      std::cout << "Ignoring the \"" << path
                << "\" folder because it doesn't have either up.sql, or "
                   "down.sql...\n";
    }
  }

  // running migrations
  bool up = *mode == migrator::Mode::RUN;

  std::string conn_url = "dbname = " + configuration->db_name +
                         " user = " + configuration->db_user +
                         " password = " + configuration->db_pass +
                         " host = " + configuration->db_host +
                         " port = " + configuration->db_port;

  pqxx::connection conn(conn_url);
  pqxx::work *work = new pqxx::work(conn);

  pqxx::result query;

  //  after we can revert the main sqlm table, we need to wrap this in a
  //  try-catch block
  try {
    query = work->exec("SELECT id FROM __sqlm_migrations");
  } catch (std::exception e) {
  }

  std::vector<std::string> migration_names;

  for (const auto &row : query) {
    migration_names.push_back(row[0].as<std::string>());
  }

  for (const std::string &path : valid_directory_paths) {
    auto path_s = migrator::utils::split_text(path, '/');
    auto name_s = migrator::utils::split_text(path_s[path_s.size() - 1], '_');
    std::string name = name_s[0];

    bool already_migrated =
        std::any_of(migration_names.begin(), migration_names.end(),
                    [&](const auto &x) { return x == name; });

    if ((up && already_migrated) || (!up && !already_migrated)) {
      continue;
    }

    std::string action;

    if (up) {
      action = "Running";
    } else {
      action = "Reverting";
    }

    std::cout << action << " the \"" << path << "\" migration...\n";

    std::string file_path;

    if (up) {
      file_path = path + "/up.sql";
    } else {
      file_path = path + "/down.sql";
    }

    std::ifstream file(file_path);

    if (!file.is_open()) {
      std::cout << "Failed to open the \"" << file_path << "\" file!\n";
      continue;
    }

    std::string contents;
    std::string line;

    while (std::getline(file, line, '\n')) {
      contents += line;
    }

    try {
      work->exec(contents);
      work->commit();
    } catch (std::exception &ex) {
      std::cout << ex.what() << "\n";
    }

    delete work;
    work = new pqxx::work(conn);
  }

  delete work;
  conn.close();

  std::cout << "Done!\n";

  return 0;
}

#include "init.hpp"

#include <filesystem>
#include <fstream>

#include "config.hpp"

namespace migrator {
  void create_sqlm_table_migration(const Configuration &c) {
    // i'm assuming the root migration folder already exists
    std::string id = "0000000000000000000";
    std::string folder_name = id + "_sqlm_init";
    std::string folder_path = c.migration_dir + "/" + folder_name;

    if (std::filesystem::exists(folder_path)) {
      return;
    }

    std::filesystem::create_directory(folder_path);

    std::ofstream up(folder_path + "/up.sql");

    up << "CREATE TABLE IF NOT EXISTS \"__sqlm_migrations\" ("
       << "\"id\" VARCHAR UNIQUE NOT NULL PRIMARY KEY,"
       << "\"run_on\" TIMESTAMP NOT NULL DEFAULT now());";

    up << "INSERT INTO \"__sqlm_migrations\"(\"id\") VALUES ("
       << "'" << id << "');";

    up.close();

    std::ofstream down(folder_path + "/down.sql");

    down << "DROP TABLE IF EXISTS \"__sqlm_migrations\";";

    down.close();
  }
}

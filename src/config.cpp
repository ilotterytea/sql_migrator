#include "config.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <optional>
#include <sstream>
#include <string>

namespace migrator {
  std::optional<Mode> parse_mode(int argc, char *argv[]) {
    if (argc == 1) {
      return std::nullopt;
    }

    // it could be done with usual char * i guess
    // im retard btw
    std::string arg = argv[1];

    if (arg == "run") {
      return Mode::RUN;
    } else if (arg == "revert") {
      return Mode::REVERT;
    }

    return std::nullopt;
  }

  std::optional<Configuration> parse_configuration_from_cli(int argc,
                                                            char *argv[]) {
    Configuration c;
    update_configuration_from_file(".env", c);

    std::map<std::string, std::string &> map = {
        {"--migration-dir", c.migration_dir},
        {"--db-name", c.db_name},
        {"--db-user", c.db_user},
        {"--db-pass", c.db_pass},
        {"--db-host", c.db_host},
        {"--db-port", c.db_port}};

    for (int i = 1; i < argc; i++) {
      std::string key, value;
      key = argv[i];

      if (key == "--help" || key == "-h") {
        std::cout << "more info\n";
        return std::nullopt;
      }

      if (key == "--config" || key == "-c") {
        if (i + 1 >= argc) {
          std::cerr << "Argument " << key << " requires a value!\n";
          return std::nullopt;
        }

        value = argv[i + 1];

        if (!update_configuration_from_file(value, c)) {
          std::cerr << "Failed to open configuration file at " << value << "\n";
          return std::nullopt;
        }

        i++;
        continue;
      }

      auto pair = std::find_if(map.begin(), map.end(),
                               [&](const auto &x) { return x.first == key; });

      if (pair == map.end()) {
        continue;
      }

      if (i + 1 >= argc) {
        std::cerr << "Argument " << key << " requires a value!\n";
        return std::nullopt;
      }

      value = argv[i + 1];

      pair->second = value;

      i++;
    }

    return c;
  }

  bool update_configuration_from_file(const std::string &path,
                                      Configuration &c) {
    std::ifstream file(path);

    if (!file.is_open()) {
      return false;
    }

    std::string line;
    while (std::getline(file, line, '\n')) {
      std::istringstream iss(line);
      std::string key, value;

      std::getline(iss, key, '=');
      std::getline(iss, value);

      if (key == "migration_dir") {
        c.migration_dir = value;
      }

      else if (key == "db_name") {
        c.db_name = value;
      } else if (key == "db_user") {
        c.db_user = value;
      } else if (key == "db_pass") {
        c.db_pass = value;
      } else if (key == "db_host") {
        c.db_host = value;
      } else if (key == "db_port") {
        c.db_port = value;
      }
    }

    return true;
  }
}

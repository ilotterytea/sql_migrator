#include "config.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

namespace migrator {
  std::optional<Mode> parse_mode(const std::vector<std::string> &args) {
    if (args.empty()) {
      return std::nullopt;
    }

    // it could be done with usual char * i guess
    // im retard btw
    std::string arg = args[0];

    if (arg == "run") {
      return Mode::RUN;
    } else if (arg == "revert") {
      return Mode::REVERT;
    }

    return std::nullopt;
  }

  std::optional<Configuration> parse_configuration_from_cli(
      std::vector<std::string> &args) {
    Configuration c;
    update_configuration_from_file(".env", c);

    std::map<std::string, std::string &> map = {
        {"--migration-dir", c.migration_dir},
        {"--db-name", c.db_name},
        {"--db-user", c.db_user},
        {"--db-pass", c.db_pass},
        {"--db-host", c.db_host},
        {"--db-port", c.db_port}};

    for (auto i = args.begin(); i != args.end();) {
      std::string key, value;
      key = *i;

      if (key == "--help" || key == "-h") {
        std::cout << "more info\n";
        return std::nullopt;
      }

      if (key == "--config" || key == "-c") {
        if (i + 1 >= args.end()) {
          std::cerr << "Argument " << key << " requires a value!\n";
          return std::nullopt;
        }

        value = *(i + 1);

        if (!update_configuration_from_file(value, c)) {
          std::cerr << "Failed to open configuration file at " << value << "\n";
          return std::nullopt;
        }

        args.erase(i, i + 1);
        continue;
      }

      auto pair = std::find_if(map.begin(), map.end(),
                               [&](const auto &x) { return x.first == key; });

      if (pair == map.end()) {
        i++;
        continue;
      }

      if (i + 1 >= args.end()) {
        std::cerr << "Argument " << key << " requires a value!\n";
        return std::nullopt;
      }

      value = *(i + 1);

      pair->second = value;

      args.erase(i, i + 2);
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

#pragma once

#include <optional>
#include <string>
#include <vector>

namespace migrator {
  struct Configuration {
      std::string migration_dir = "migrations";
      std::string db_name, db_user, db_pass;
      std::string db_host = "localhost", db_port = "5432";
  };

  enum Mode { RUN, REVERT, GENERATE };

  std::optional<Mode> parse_mode(const std::vector<std::string> &args);
  std::optional<Configuration> parse_configuration_from_cli(
      std::vector<std::string> &args);
  bool update_configuration_from_file(const std::string &path,
                                      Configuration &c);

  void show_more_info();
}

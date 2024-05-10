#pragma once

#include <optional>
#include <string>

namespace migrator {
  struct Configuration {
      std::string migration_dir = "migrations";
      std::string db_name, db_user, db_pass;
      std::string db_host = "localhost", db_port = "5432";
  };

  enum Mode { RUN, REVERT };

  std::optional<Mode> parse_mode(int argc, char *argv[]);
  std::optional<Configuration> parse_configuration_from_cli(int argc,
                                                            char *argv[]);
  bool update_configuration_from_file(const std::string &path,
                                      Configuration &c);
}

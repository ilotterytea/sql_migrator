#pragma once

#include <string>
#include <vector>

#include "config.hpp"
namespace migrator {
  void generate_migration_folder(const Configuration &configuration,
                                 const std::vector<std::string> &args);
}

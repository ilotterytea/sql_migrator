#pragma once

#include <string>
#include <vector>

namespace migrator::utils {
  std::vector<std::string> split_text(const std::string &text, char delimiter);
}

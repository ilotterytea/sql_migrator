#include "config.hpp"

#include <iostream>
#include <optional>
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
}

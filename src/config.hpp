#pragma once

#include <optional>

namespace migrator {
  enum Mode { RUN, REVERT };

  std::optional<Mode> parse_mode(int argc, char *argv[]);
}

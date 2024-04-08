/**
 * @copryright
 * Copyright 2023 Adam-Al-Rahman
 */

#include "netzwerx/core/privilege/super_user.h"

#include <unistd.h>

#include <iostream>
#include <string>

namespace netzwerx::core {

[[nodiscard]] std::string super_user() {
  if (geteuid() != 0) {
    std::cout
        << "[-] This program requires sudo privileges. Please run with sudo.\n";
    return "!sudo";
  }
  return "success";
}

}  // namespace netzwerx::core

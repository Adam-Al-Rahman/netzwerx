
#include "super_user.h"

#include <unistd.h>

#include <iostream>

namespace netzwerx::core {

std::string super_user() {
  if (geteuid() != 0) {
    std::cout
        << "[-] This program requires sudo privileges. Please run with sudo.\n";
    return "!sudo";
  }
  return "success";
};

}  // namespace netzwerx::core
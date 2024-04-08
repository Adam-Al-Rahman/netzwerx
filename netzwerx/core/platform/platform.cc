/**
 * Copyright 2023 Adam-Al-Rahman
 */

#include "netzwerx/core/platform/platform.h"

#include <cstddef>
#include <fstream>
#include <string>

#include "absl/strings/match.h"

namespace netzwerx::core {

/**
 * The function returns the operating system name based on the preprocessor
 * directives.
 *
 * @return The function `get_operating_system()` returns a string indicating the
 * operating system. The possible return values are "window" for Windows,
 * "macOS" for Apple operating systems, "linux" for Linux, and "Unknown" if the
 * operating system is not recognized.
 */
std::string get_operating_system() {
#if defined(_WIN32) || defined(_WIN64)
  return "window";
#elif defined(__APPLE__)
  // TODO(Adam-Al-Rahman): write the functions for macOS
  return "macOS";
#elif defined(__linux__)
  return "linux";
#else
  return "unknown";
#endif
}

/**
 * The function `get_linux_distribution()` reads the `/etc/os-release` file in
 * Linux and returns the distribution name.
 *
 * @return a string that represents the Linux distribution name.
 */
std::string get_linux_distribution() {
  std::ifstream os_release("/etc/os-release");
  std::string line;
  std::string distribution_name;

  if (os_release.is_open()) {
    while (std::getline(os_release, line)) {
      if (absl::StrContains(line, "ID_LIKE=")) {
        const std::size_t position = line.find('=');
        if (position != std::string::npos) {
          distribution_name = line.substr(position + 1);
          break;
        }
      }
    }
    os_release.close();
  }

  // Trim any potential leading or trailing spaces
  distribution_name.erase(0, distribution_name.find_first_not_of(" \t\r\n"));
  distribution_name.erase(distribution_name.find_last_not_of(" \t\r\n") + 1);

  return distribution_name;
}

/**
 * The function returns the operating system and, if it's Linux, also the Linux
 * distribution.
 *
 * @return a string that represents the operating system platform.
 */
std::string platform() {
  std::string os = get_operating_system();

  // TODO(Adam-Al-Rahman): Remove it, if(no need of specific linux distribution
  // name);
  // if (os == "linux") {
  //   os.assign(get_linux_distribution());
  // }
  return os;
}

}  // namespace netzwerx::core

#ifndef PLATFORM_H_
#define PLATFORM_H_

#include <string>

namespace netzwerx::core {

std::string get_operating_system();
std::string get_linux_distribution();

std::string platform();

}  // namespace netzwerx::core

#endif  // PLATFORM_H_
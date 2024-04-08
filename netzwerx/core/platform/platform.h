/**
 * Copyright 2023 Adam-Al-Rahman
 */

#ifndef NETZWERX_CORE_PLATFORM_PLATFORM_H_
#define NETZWERX_CORE_PLATFORM_PLATFORM_H_

#include <string>

namespace netzwerx::core {

std::string get_operating_system();
std::string get_linux_distribution();

std::string platform();

}  // namespace netzwerx::core

#endif  // NETZWERX_CORE_PLATFORM_PLATFORM_H_

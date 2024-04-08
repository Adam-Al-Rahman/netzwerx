/**
 * Copyright 2023 Netzwerx
 */

#ifndef NETZWERX_BACKEND_UTILS_MAC_ADDRESS_UTILS_H_
#define NETZWERX_BACKEND_UTILS_MAC_ADDRESS_UTILS_H_

#include <string>
#include <vector>

namespace netzwerx::backend {

std::string regex_mac_address(const std::string& new_mac_address);
bool is_locally_administered(const std::string& mac_address);
int linux_execute_command(const std::vector<const char*>& command_args);
bool mac_starts_00(const std::string& new_mac_address);

std::vector<std::string> get_interfaces_with_mac();
std::vector<std::string> get_linux_network_interfaces(bool with_mac = false);

}  // namespace netzwerx::backend

#endif  // NETZWERX_BACKEND_UTILS_MAC_ADDRESS_UTILS_H_

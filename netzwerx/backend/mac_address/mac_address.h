/**
 * Copyright 2023 Netzwerx
 */
#ifndef NETZWERX_BACKEND_MAC_ADDRESS_MAC_ADDRESS_H_
#define NETZWERX_BACKEND_MAC_ADDRESS_MAC_ADDRESS_H_

#include <string>

namespace netzwerx::backend {

[[nodiscard]] std::string maddress_config_linux(
    const std::string& interface, const std::string& new_mac_address);
[[nodiscard]] std::string maddress_config_windows(
    const std::string& interface, const std::string& new_mac_address);

[[nodiscard]] std::string get_mac_address(const std::string& interface);

[[nodiscard]] std::string mac_changer(const std::string& interface,
                                      const std::string& new_mac_address);

}  // namespace netzwerx::backend

#endif  // NETZWERX_BACKEND_MAC_ADDRESS_MAC_ADDRESS_H_

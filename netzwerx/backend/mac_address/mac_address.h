#ifndef MAC_ADDRESS_H_
#define MAC_ADDRESS_H_

#include <iostream>
#include <string>
#include <tuple>

namespace netzwerx::backend {

std::string mac_linux_debian(const std::string& interface,
                             const std::string& new_mac_address);
std::string mac_linux_arch(const std::string& interface,
                           const std::string& new_mac_address);
std::string mac_linux_windows(const std::string& interface,
                              const std::string& new_mac_address);

std::string get_mac_address(const std::string& interface);

std::string mac_changer(const std::string& interface,
                        const std::string& new_mac_address);

}  // namespace netzwerx::backend

#endif  // MAC_ADDRESS_H_

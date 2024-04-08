/**
 * Copyright 2023 Netzwerx
 */

#ifndef NETZWERX_BACKEND_MAC_ADDRESS_MAC_ADDRESS_MOCK_H_
#define NETZWERX_BACKEND_MAC_ADDRESS_MAC_ADDRESS_MOCK_H_

#include <string>
#include <vector>

#include "gmock/gmock.h"
#include "subprocess/subprocess.hpp"

namespace netzwerx::backend {
// TEST(Suite Name, Test name)

class MacLinuxInnerMethod {
 public:
  virtual ~MacLinuxInnerMethod() = default;
  [[nodiscard]] virtual std::string super_user() const = 0;
  [[nodiscard]] virtual std::vector<std::string> get_linux_network_interfaces()
      const = 0;
};

class MockMacLinuxInnerMethod : public MacLinuxInnerMethod {
 public:
  MOCK_CONST_METHOD0(super_user, std::string());
  MOCK_CONST_METHOD0(get_linux_network_interfaces, std::vector<std::string>());
};

inline std::string maddress_config_linux(const std::string& interface,
                                         const std::string& new_mac_address,
                                         const MacLinuxInnerMethod& method) {
  std::string super_user_privilege = method.super_user();

  if (super_user_privilege == "!sudo") {
    return super_user_privilege;
  }

  std::vector<std::string> interfaces = method.get_linux_network_interfaces();
  if (std::find(interfaces.begin(), interfaces.end(), interface) !=
      interfaces.end()) {
    subprocess::call({"ip", "link", "set", "dev", interface, "down"});
    subprocess::call(
        {"ip", "link", "set", "dev", interface, "address", new_mac_address});
    subprocess::call({"ip", "link", "set", "dev", interface, "up"});

  } else {
    std::cerr << "Interface " << interface << " not found." << '\n';
    return "!" + interface;
  }

  return "success";
}

}  // namespace netzwerx::backend

#endif  // NETZWERX_BACKEND_MAC_ADDRESS_MAC_ADDRESS_MOCK_H_

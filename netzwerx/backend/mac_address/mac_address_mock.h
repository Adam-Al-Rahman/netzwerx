#ifndef MAC_ADDRESS_MOCK_H_
#define MAC_ADDRESS_MOCK_H_

#include "gmock/gmock.h"
#include "netzwerx/core/platform/platform.h"

namespace netzwerx::backend {
// TEST(Suite Name, Test name)

class MacLinuxInnerMethod {
 public:
  virtual ~MacLinuxInnerMethod() = default;
  [[nodiscard]] virtual std::string super_user() const = 0;
  [[nodiscard]] virtual std::vector<std::string> get_linux_network_interfaces()
      const = 0;
  [[nodiscard]] virtual int linux_execute_command(
      const std::vector<const char*>&) const = 0;
};

class MockMacLinuxInnerMethod : public MacLinuxInnerMethod {
 public:
  MOCK_CONST_METHOD0(super_user, std::string());
  MOCK_CONST_METHOD0(get_linux_network_interfaces, std::vector<std::string>());
  MOCK_CONST_METHOD1(linux_execute_command,
                     int(const std::vector<const char*>&));
};

std::string mac_linux_debian(const std::string& interface,
                             const std::string& new_mac_address,
                             const MacLinuxInnerMethod& method) {
  std::string super_user_privilege = method.super_user();

  if (super_user_privilege == "!sudo") {
    return super_user_privilege;
  }

  std::vector<std::string> interfaces = method.get_linux_network_interfaces();
  if (std::find(interfaces.begin(), interfaces.end(), interface) !=
      interfaces.end()) {
    std::vector<const char*> commands[] = {
        {"ifconfig", interface.c_str(), "down", nullptr},
        {"ifconfig", interface.c_str(), "hw", "ether", new_mac_address.c_str(),
         nullptr},
        {"ifconfig", interface.c_str(), "up", nullptr}};

    for (const auto& command_args : commands) {
      int result = method.linux_execute_command(command_args);
      if (result != 0) {
        std::cerr << "Error executing command. Aborting." << std::endl;
        return "error";
      }
    }
  } else {
    std::cerr << "Interface " << interface << " not found." << std::endl;
    return "!" + interface;
  }

  return "success";
}

std::string mac_linux_arch(const std::string& interface,
                           const std::string& new_mac_address,
                           const MacLinuxInnerMethod& method) {
  std::string super_user_privilege = method.super_user();

  if (super_user_privilege == "!sudo") {
    return super_user_privilege;
  }

  std::vector<std::string> interfaces = method.get_linux_network_interfaces();
  if (std::find(interfaces.begin(), interfaces.end(), interface) !=
      interfaces.end()) {
    std::vector<const char*> commands[] = {
        {"ip", "link", "set", "dev", interface.c_str(), "down", nullptr},
        {"ip", "link", "set", "dev", interface.c_str(), "address",
         new_mac_address.c_str(), nullptr},
        {"ip", "link", "set", "dev", interface.c_str(), "up", nullptr}};

    for (const auto& command_args : commands) {
      int result = method.linux_execute_command(command_args);
      if (result != 0) {
        std::cerr << "Error executing command. Aborting." << std::endl;
        return "error";
      }
    }
  } else {
    std::cerr << "Interface " << interface << " not found." << std::endl;
    return "!" + interface;
  }

  return "success";
}

}  // namespace netzwerx::backend

#endif  // MAC_ADDRESS_MOCK_H_
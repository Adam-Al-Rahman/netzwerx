/**
 * Copyright 2023 Netzwerx
 */

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>  // https://stackoverflow.com/a/15466951/14925917
#endif

#include <algorithm>
#include <cstddef>
#include <cstdio>
#include <exception>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

#include "netzwerx/backend/mac_address/mac_address.h"
#include "netzwerx/backend/utils/mac_address_utils.h"
#include "netzwerx/core/platform/platform.h"
#include "subprocess/subprocess.hpp"

namespace netzwerx::backend {

/**
 * The function `maddress_config_linux` takes an interface name and a new MAC
 * address as input, and uses system commands to change the MAC address of the
 * specified interface on a Linux system.
 *
 * @param interface The interface parameter is a string that represents the
 * network interface on the system. It could be something like "eth0" or
 * "wlan0".
 * @param new_mac_address The `new_mac_address` parameter is a string that
 * represents the new MAC address that you want to assign to the network
 * interface. A MAC address is a unique identifier assigned to network
 * interfaces, and it is usually represented as a sequence of six pairs of
 * hexadecimal digits separated by colons or hyphens
 *
 * @return an integer value, which is the system code.
 */

[[nodiscard]] std::string maddress_config_linux(
    const std::string& interface, const std::string& new_mac_address) {
  // TODO(Adam-Al-Rahman): run this function with specifying sudo
  // std::string super_user_privilege = netzwerx::core::super_user();

  // if (super_user_privilege == "!sudo") {
  //   return super_user_privilege;
  // }
  //

  std::vector<std::string> interfaces = get_linux_network_interfaces(true);
  if (std::find(interfaces.begin(), interfaces.end(), interface) !=
      interfaces.end()) {
    // Commands to change the MAC address using 'ip' command
    subprocess::call({"sudo", "ip", "link", "set", "dev", interface, "down"});
    subprocess::call({"sudo", "ip", "link", "set", "dev", interface, "address",
                      new_mac_address});
    subprocess::call({"sudo", "ip", "link", "set", "dev", interface, "up"});

  } else {
    std::cerr << "Interface " << interface << " not found." << '\n';
    return "!" + interface;
  }

  return "success";
}

// TODO(Adam-Al-Rahman):  handle <windows.h>
[[nodiscard]] std::string maddress_config_windows(
    const std::string& interface, const std::string& new_mac_address) {
  std::cout << "Interface: " << interface << " || "
            << "New MAC Address: " << new_mac_address << '\n';

#if defined(_WIN32) || defined(_WIN64)

  // Construct the PowerShell command
  std::wstring command_line =
      L"powershell -Command Set-NetAdapter -Name \"" +
      std::wstring(interface.begin(), interface.end()) + L"\" -MacAddress \"" +
      std::wstring(new_mac_address.begin(), new_mac_address.end()) + L"\"";

  // Create process information
  PROCESS_INFORMATION process_info;
  STARTUPINFO startup_info;

  ZeroMemory(&startup_info, sizeof(startup_info));
  startup_info.cb = sizeof(startup_info);
  ZeroMemory(&process_info, sizeof(process_info));

  // Create the process
  if (!CreateProcess(nullptr, const_cast<wchar_t*>(command_line.c_str()),
                     nullptr, nullptr, FALSE, 0, nullptr, nullptr,
                     &startup_info, &process_info)) {
    std::cerr << "CreateProcess failed (" << GetLastError() << ")."
              << std::endl;
    return "error";
  }

  // Wait until child process exits.
  WaitForSingleObject(process_info.hProcess, INFINITE);

  // Get the exit code of the process
  DWORD exitCode = 0;
  GetExitCodeProcess(process_info.hProcess, &exitCode);

  // Close process and thread handles.
  CloseHandle(process_info.hProcess);
  CloseHandle(process_info.hThread);

  // Check the exit code
  if (exitCode != 0) {
    std::cerr << "Error executing PowerShell command." << std::endl;
    return {static_cast<int>(exitCode), "error"};
  }

  return "success";

#else
  // Dummy implementation for non-Windows platforms
  std::cerr << R"(
    [-] NON WINDOWS PLATFORMS
    fn: mac_windows
    Reason: This function only works on windows platforms)"
            << '\n';
  return "!windows";
#endif
}

/**
 * The function `get_mac_address` retrieves the MAC address of a network
 * interface based on the platform and interface provided.
 *
 * @param platform The "platform" parameter is a string that specifies the
 * operating system platform. It can have one of the following values:
 * "linux", or "windows".
 * @param interface The `interface` parameter is a string that represents the
 * name of the network interface for which you want to retrieve the MAC
 * address. Examples of interface names could be "eth0" or "wlan0".
 *
 * @return The function `get_mac_address` returns the MAC address of a network
 * interface as a `std::string`. If the MAC address is found, it returns the
 * MAC address as a string of 17 characters. If the MAC address is not found,
 * it returns the string "MAC address not found".
 */

[[nodiscard]] std::string get_mac_address(const std::string& interface) {
  const std::string platform = netzwerx::core::platform();
  std::string command;

  if (platform == "linux") {
    command = "ip link show " + interface;
  } else if (platform == "windows") {
    command = "ipconfig /all";
  } else {
    throw std::invalid_argument("Unsupported platform");
  }

  // Use std::filesystem for file I/O
  std::string result;
  try {
    result =
        subprocess::check_output({"ip", "link", "show", interface}).buf.data();
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << '\n';
    return "!" + interface;
  }

  std::size_t pos;
  if (platform == "linux") {
    pos = result.find("ether");
    if (pos != std::string::npos) {
      pos += 6;                       // Move past "ether "
      return result.substr(pos, 17);  // MAC addresses are 17 characters long
    }
  } else if (platform == "windows") {
    pos = result.find("Physical Address");
    if (pos != std::string::npos) {
      pos += 36;  // Move past "Physical Address. . . . . . . . . : "
      return result.substr(pos, 17);  // MAC addresses are 17 characters long
    }
  }

  return "!" + interface;
}

/**
 * The function `mac_changer` changes the MAC address of a network interface
 * based on the operating system.
 *
 * @param interface The "interface" parameter is a string that represents the
 * network interface on which you want to change the MAC address. It could be
 * the name of a network adapter, such as "eth0" or "wlan0", depending on the
 * operating system you are using.
 * @param new_mac_address The `new_mac_address` parameter is a `std::string`
 * that represents the new MAC address that you want to set for the specified
 * network interface.
 *
 * @return The function `mac_changer` returns an integer value.
 */

[[nodiscard]] std::string mac_changer(const std::string& interface,
                                      const std::string& new_mac_address) {
  const std::string os = netzwerx::core::platform();

  const std::string filtered_mac_address = regex_mac_address(new_mac_address);
  if (filtered_mac_address != "unknown" &&
      (is_locally_administered(filtered_mac_address) ||
       mac_starts_00(filtered_mac_address))) {
    // Create a map that associates OS names with their respective functions
    std::unordered_map<
        std::string,
        std::function<std::string(const std::string&, const std::string&)>>
        os_fn = {
            {"linux", maddress_config_linux},
            {"windows", maddress_config_windows},
        };

    auto it = os_fn.find(os);
    if (it != os_fn.end()) {
      return it->second(interface, filtered_mac_address);
    }
    // Handle the case where the OS is not supported
    std::cerr << "Unsupported OS: " << os << '\n';
    return "!" + os;
  }
  // Handle the case where the MAC address is invalid
  std::cerr << "Invalid MAC address: " << filtered_mac_address << '\n';
  return "!" + new_mac_address;
}

}  // namespace netzwerx::backend

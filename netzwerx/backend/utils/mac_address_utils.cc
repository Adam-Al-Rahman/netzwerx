/**
 * Copyright 2023 Netzwerx
 */

#include "netzwerx/backend/utils/mac_address_utils.h"

#include <ifaddrs.h>
#include <net/if.h>
#include <sched.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <set>
#include <string>
#include <vector>

// TODO(Adam-Al-Rahman): use https://github.com/google/re2
// #include <regex>

#include "re2/re2.h"

namespace netzwerx::backend {
std::string regex_mac_address(const std::string& new_mac_address) {
  // Define the regular expression pattern for a MAC address

  std::string pattern;
  pattern = "^(([0-9A-Fa-f]{2}[:-]){5}([0-9A-Fa-f]{2}))";

  const RE2 mac_regex(pattern);

  // Search for the MAC address in the new_mac_address string
  std::string extracted_mac;
  if (RE2::FullMatch(new_mac_address, mac_regex, &extracted_mac)) {
    return extracted_mac;
  }

  return "unknown";
}

bool is_locally_administered(const std::string& mac_address) {
  // https://www.wikiwand.com/en/MAC_address#Ranges_of_group_and_locally_administered_addresses

  // Extract the first byte from the MAC address
  const std::string first_byte = mac_address.substr(0, 2);

  // Convert the first byte to an integer
  const int first_byte_value = std::stoi(first_byte, nullptr, 16);

  // Check if the second-least significant bit is set to 1
  return ((first_byte_value & 2) == 2);
}

bool mac_starts_00(const std::string& new_mac_address) {
  // Check if the MAC address starts with "00"
  return (new_mac_address.size() >= 2 && new_mac_address.substr(0, 2) == "00");
}

int linux_execute_command(const std::vector<const char*>& command_args) {
  int status = 0;
  const pid_t child_pid = fork();

  if (child_pid == 0) {  // This is the child process
    execvp(command_args[0], const_cast<char* const*>(command_args.data()));

    // If execvp fails
    std::cerr << "Failed to execute command" << '\n';
    _exit(EXIT_FAILURE);

  } else if (child_pid > 0) {  // This is the parent process
    waitpid(child_pid, &status, 0);

    if (status != 0) {
      std::cerr << "Command failed: " << command_args[0] << '\n';
      return status;
    }
  } else {  // Fork failed
    std::cerr << "Fork failed" << '\n';
    return -1;
  }

  return 0;
}

std::vector<std::string> get_interfaces_with_mac() {
  std::vector<std::string> interface_names;
  const int sock = socket(AF_INET, SOCK_DGRAM, 0);

  if (sock < 0) {
    std::perror("socket");
    return interface_names;
  }

  struct if_nameindex* if_ni = if_nameindex();
  if (if_ni == nullptr) {
    std::perror("if_nameindex");
    close(sock);
    return interface_names;
  }

  for (struct if_nameindex* i = if_ni; i->if_name != nullptr; i++) {
    struct ifreq ifr;
    std::memset(&ifr, 0, sizeof(ifr));
    std::snprintf(ifr.ifr_name, IFNAMSIZ, "%s", i->if_name);

    if (ioctl(sock, SIOCGIFHWADDR, &ifr) < 0) {
      perror("ioctl");
      continue;
    }

    auto* mac = reinterpret_cast<unsigned char*>(ifr.ifr_hwaddr.sa_data);
    const bool is_loopback = std::all_of(
        mac, mac + 6, [](unsigned char byte) { return byte == 0x00; });

    if (!is_loopback) {
      interface_names.emplace_back(i->if_name);
    }
  }

  if_freenameindex(if_ni);
  close(sock);
  return interface_names;
}

std::vector<std::string> get_linux_network_interfaces(bool with_mac) {
  // Return interface which has mac address
  if (with_mac) {
    return get_interfaces_with_mac();
  }

  std::vector<std::string> interface_list;
  std::set<std::string> seen_interfaces;  // To keep track of interfaces

  struct ifaddrs* if_addr_struct = nullptr;
  struct ifaddrs* ifa = nullptr;

  // Get the list of interface addresses
  if (getifaddrs(&if_addr_struct) == -1) {
    std::cerr << "Error getting interface addresses" << '\n';
    return interface_list;
  }

  // Iterate through the list of interface addresses
  for (ifa = if_addr_struct; ifa != nullptr; ifa = ifa->ifa_next) {
    if (ifa->ifa_addr == nullptr) continue;

    // Check if it's an IPv4 or IPv6 address
    if (ifa->ifa_addr->sa_family == AF_INET ||
        ifa->ifa_addr->sa_family == AF_INET6) {
      // Add the interface name to the list if not seen before
      if (seen_interfaces.find(ifa->ifa_name) == seen_interfaces.end()) {
        interface_list.emplace_back(ifa->ifa_name);
        seen_interfaces.insert(ifa->ifa_name);
      }
    }
  }

  // Free the memory allocated by getifaddrs
  freeifaddrs(if_addr_struct);

  return interface_list;
}

// TODO(Adam-Al-Rahman): complete generate_random_la_mac
// std::string generate_random_la_mac(std::string format = '',
//                                    std::random_device rd) {
//   // Define the locally administered MAC address formats
//   const std::string formats[] = {"X2", "X6", "XA", "XE"};

//   // Seed for the random number generator
//   std::random_device rd;
//   std::mt19937 gen(rd());

//   // Randomly select a format
//   std::uniform_int_distribution<> format_dist(
//       0, sizeof(formats) / sizeof(formats[0]) - 1);

//   const std::string selected_format = formats[format_dist(gen)];

//   // Function to generate a random hexadecimal digit
//   auto random_hex_digit = [&gen]() -> char {
//     std::uniform_int_distribution<> dist(0, 15);
//     const int value = dist(gen);
//     if (value < 10) {
//       return static_cast<char>('0' + value);
//     }
//     return static_cast<char>('A' + value - 10);
//   };

//   // Replace 'X' with random hexadecimal digits
//   std::string mac_address;
//   for (char format_char : selected_format) {
//     if (format_char == 'X') {
//       mac_address += random_hex_digit();
//     } else {
//       mac_address += format_char;
//     }
//   }

//   return mac_address;
// }
}  // namespace netzwerx::backend

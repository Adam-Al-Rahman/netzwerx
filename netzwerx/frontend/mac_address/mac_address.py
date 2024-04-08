# Netzwerx, Apache 2 license

import argparse

from netzwerx.frontend.mac_address import _pywrap_mac_address  # type: ignore


def get_mac_address_arguments():
  """
  The function `get_mac_address_arguments()` is used to parse command line arguments for changing the
  MAC address of a network interface.
  :return: the options object, which contains the values of the parsed command-line arguments.
  """
  parser = argparse.ArgumentParser(description='Change MAC address of a network interface.')
  parser.add_argument(
    '-i',
    '--interface',
    dest='interface',
    help='Interface to change its MAC address',
    required=True,
  )
  parser.add_argument(
    '-m',
    '--mac',
    dest='new_mac_address',
    help='New MAC address',
    required=True,
  )

  options = parser.parse_args()

  return options


def get_mac_address(interface: str) -> str:
  """
  The function `get_mac_address` returns the MAC address of a given network interface.

  :param interface: The "interface" parameter is a string that represents the network interface for
  which you want to retrieve the MAC address. Examples of network interfaces include "eth0" for
  Ethernet, "wlan0" for Wi-Fi, and "lo" for the loopback interface
  :type interface: str
  :return: a string, which is the MAC address of the specified network interface.
  """
  return _pywrap_mac_address.get_mac_address(interface)


def mac_changer(interface: str, new_mac_address: str) -> None:
  """
  The function `mac_changer` changes the MAC address of a network interface and prints the updated MAC
  address.

  :param interface: The "interface" parameter is a string that represents the network interface on
  which you want to change the MAC address. Examples of interface names are "eth0" for Ethernet and
  "wlan0" for Wi-Fi
  :type interface: str
  :param new_mac_address: The `new_mac_address` parameter is a string that represents the new MAC
  address that you want to set for the specified network interface
  :type new_mac_address: str
  """

  extract_mac_address = _pywrap_mac_address.regex_mac_address(new_mac_address)

  if (extract_mac_address == 'unknown'):
    print('[-] Incorrect MAC Address\nNetzwerX perform FullMatch Regex')
  else:
    print(f'[+] Changing MAC address for {interface} to {extract_mac_address}')

    # not because mac_changer return 0 if it change the mac address
    if _pywrap_mac_address.mac_changer(interface, new_mac_address) == 'sucess':
      print(f'[+] Updated MAC Address: {get_mac_address(interface)}')
    elif _pywrap_mac_address.mac_changer(interface, new_mac_address) == f'!{new_mac_address}':
      print(
        f'[-] MAC Address: {extract_mac_address} is not locally administered\nRead: https://www.wikiwand.com/en/MAC_address#Ranges_of_group_and_locally_administered_addresses\nFor simplicity\nUse MAC Address Format(LINUX): 00:XX:XX:XX:XX:XX\nUse MAC Address Format(Windows): 00-XX-XX-XX-XX-XX'
      )


print('___Netzwerx___')

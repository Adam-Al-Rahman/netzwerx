def mac_changer(arg0: str, arg1: str) -> str:
  """
  The function `mac_changer` takes an interface name and a new MAC address as input and returns a
  string.

  :param interface: The "interface" parameter is a string that represents the network interface on the
  device. This could be the name of a network adapter, such as "eth0" or "wlan0", or it could be the
  name of a virtual interface, such as "tun0" or "br
  :type interface: str
  :param new_mac_address: The `new_mac_address` parameter is a string that represents the new MAC
  address that you want to assign to the network interface. A MAC address is a unique identifier
  assigned to network interfaces, and it is usually represented as a series of six pairs of
  hexadecimal digits separated by colons or hyphens
  :type new_mac_address: str
  """
  ...


def get_mac_address(arg0: str) -> str:
  """
  The function `get_mac_address` takes in a string parameter `interface` and returns a string
  representing the MAC address of the specified network interface.

  :param interface: The `interface` parameter is a string that represents the network interface for
  which you want to retrieve the MAC address. Examples of network interfaces include "eth0" for
  Ethernet, "wlan0" for Wi-Fi, and "lo" for the loopback interface
  :type interface: str
  """
  ...

def regex_mac_address(arg0: str) -> str:
  ...
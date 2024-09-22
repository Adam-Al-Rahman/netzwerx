# Netzwerx, Apache 2 license

import argparse


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
    Returns the MAC address of a given network interface.
    """
  from netzwerx.frontend.mac import _pywrap_mac_address  # Delay import

  return _pywrap_mac_address.get_mac_address(interface)


def mac_changer(interface: str, new_mac_address: str) -> None:
  """
    Changes the MAC address of a network interface.
    """
  from netzwerx.frontend.mac import _pywrap_mac_address  # Delay import

  extract_mac_address = _pywrap_mac_address.regex_mac_address(new_mac_address)

  if extract_mac_address == 'unknown':
    print('[-] Incorrect MAC Address\nSanitization: NetzwerX perform FullMatch Regex')
    return

  print(f'[+] Changing MAC address for {interface} to {extract_mac_address}')
  change_result = _pywrap_mac_address.mac_changer(interface, new_mac_address)

  if change_result == 'success':
    print(f'[+] Updated MAC Address: {get_mac_address(interface)}')
  elif change_result == f'!{new_mac_address}':
    print(f'[-] MAC Address: {extract_mac_address} is not locally administered\n'
          'Read: https://www.wikiwand.com/en/MAC_address#Ranges_of_group_and_locally_administered_addresses\n'
          'For simplicity\n'
          'Use MAC Address Format(LINUX): 00:XX:XX:XX:XX:XX\n'
          'Use MAC Address Format(Windows): 00-XX-XX-XX-XX-XX')

from typing import Callable, Dict

from netzwerx.frontend.mac.mac_address import get_mac_address, mac_changer


def get_mode_args():
  return [{'mode': 'maddr', 'args': ['if', 'nm']}, {'mode': 'nscan', 'args': []}]


def abbreviation_to_original(args: Dict[str, str]) -> Dict[str, str]:
  abbreviations = {'if': 'interface', 'nm': 'new_mac_address'}

  converted_args = {}
  for key, value in args.items():
    converted_key = abbreviations.get(key, key)  # Check if original exists, else keep the abbreviation key
    converted_args[converted_key] = value

  return converted_args


def select_mode_fn(mode: str, args: Dict[str, str]) -> Callable[..., str]:
  # choose which method to use if arg provided else define a default method
  modes_fns = {
    'maddr': mac_changer if args else get_mac_address,
    'nscan': get_mac_address}  # TODO: change 'nscan' method

  selected_fn = modes_fns.get(mode)
  if not selected_fn:
    raise ValueError(f"Mode '{mode}' is not supported.")

  return selected_fn(**abbreviation_to_original(args)) if args else selected_fn()

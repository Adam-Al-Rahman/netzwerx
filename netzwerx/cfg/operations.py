from typing import Callable, Dict

from netzwerx.frontend.mac_address.mac_address import get_mac_address, mac_changer


def get_mode_args():
  return [{'mode': 'maddr', 'args': ['if', 'nw']}, {'mode': 'nscan', 'args': []}]


def abbreviation_to_original(args: Dict[str, str]) -> Dict[str, str]:
  abbreviations = {'if': 'interface', 'nw': 'new_mac_address'}

  converted_args = {}
  for key, value in args.items():
    converted_key = abbreviations.get(key, key)  # Check if original exists, else keep the abbreviation key
    converted_args[converted_key] = value

  return converted_args


def select_mode_fn(mode: str, args: Dict[str, str]) -> Callable[..., str]:
  modes_fns = {'maddr': mac_changer if args else get_mac_address}

  selected_fn = modes_fns.get(mode)
  if selected_fn:
    return selected_fn(**abbreviation_to_original(args)) if args else selected_fn()
  else:
    raise ValueError(f"Mode '{mode}' is not supported.")

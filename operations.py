from typing import Dict


def mac_changer(interface, new_mac_address):
  return f"Interface: {interface}, new mac: {new_mac_address}"


def get_mac_address():
  return "00:22:33:44:55:11"


def convert_args(args_dict):
  abbreviation_dict = {"if": "interface", "nw": "new_mac_address"}

  converted_args = {}
  for key, value in args_dict.items():
    converted_key = abbreviation_dict.get(key, key)  # Check if abbreviation exists, else keep the original key
    converted_args[converted_key] = value

  return converted_args


def select_mode_fn(mode: str, args: Dict):
  modes_fns = {"maddr": mac_changer if args else get_mac_address}

  selected_function = modes_fns.get(mode)
  if selected_function:
    return selected_function(**convert_args(args)) if args else selected_function()
  else:
    raise ValueError(f"Mode '{mode}' is not supported.")

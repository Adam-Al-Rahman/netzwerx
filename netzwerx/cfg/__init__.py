import sys
import contextlib
import logging.config

from typing import List, Dict, Union

from netzwerx import __version__
from netzwerx.cfg.msg import CLI_ARGS
from netzwerx.cfg.operations import select_mode_fn

# Define valid modes
MODES = 'maddr', 'nscan'

CLI_HELP_MSG = \
    f"""
    Arguments received: {str(['netzwerx'] + sys.argv[1:])}. Netzwerx 'netzwerx' commands use the following syntax:

        netzwerx MODE ARGS

        Where   MODE (required) is one of {MODES}
                ARGS (required) are any number of custom 'arg=value'
                See all {MODES} Respectives ARGS with 'netzwerx args'

    1. Change mac address of a specific interface
        netzwerx maddr if=eth0 nm=00:11:22:33:44:55

    2. Run special commands:
        netzwerx help
        netzwerx version
        netzwerx args

    Docs: https://github.com/Adam-Al-Rahman/netzwerx#readme
    GitHub: https://github.com/Adam-Al-Rahman/netzwerx
    """

# Set logger
LOGGING_NAME = "netzwerx"
LOGGER = logging.getLogger(LOGGING_NAME)


def colorstr(*input):
  """
    Colors a string based on the provided color and style arguments. Utilizes ANSI escape codes.
    See https://en.wikipedia.org/wiki/ANSI_escape_code for more details.

    This function can be called in two ways:
        - colorstr('color', 'style', 'your string')
        - colorstr('your string')

    In the second form, 'blue' and 'bold' will be applied by default.

    Args:
        *input (str): A sequence of strings where the first n-1 strings are color and style arguments,
                      and the last string is the one to be colored.

    Supported Colors and Styles:
        Basic Colors: 'black', 'red', 'green', 'yellow', 'blue', 'magenta', 'cyan', 'white'
        Bright Colors: 'bright_black', 'bright_red', 'bright_green', 'bright_yellow',
                       'bright_blue', 'bright_magenta', 'bright_cyan', 'bright_white'
        Misc: 'end', 'bold', 'underline'

    Returns:
        (str): The input string wrapped with ANSI escape codes for the specified color and style.

    Examples:
        >>> colorstr('blue', 'bold', 'hello world')
        >>> '\033[34m\033[1mhello world\033[0m'
    """
  *args, string = input if len(input) > 1 else ('blue', 'bold', input[0])  # color arguments, string
  colors = {
      'black': '\033[30m',  # basic colors
      'red': '\033[31m',
      'green': '\033[32m',
      'yellow': '\033[33m',
      'blue': '\033[34m',
      'magenta': '\033[35m',
      'cyan': '\033[36m',
      'white': '\033[37m',
      'bright_black': '\033[90m',  # bright colors
      'bright_red': '\033[91m',
      'bright_green': '\033[92m',
      'bright_yellow': '\033[93m',
      'bright_blue': '\033[94m',
      'bright_magenta': '\033[95m',
      'bright_cyan': '\033[96m',
      'bright_white': '\033[97m',
      'end': '\033[0m',  # misc
      'bold': '\033[1m',
      'underline': '\033[4m'}
  return ''.join(colors[x] for x in args) + f'{string}' + colors['end']


def parse_key_value_pair(pair):
  """Parse one 'key=value' pair and return key and value."""
  k, v = pair.split('=', 1)  # split on first '=' sign
  k, v = k.strip(), v.strip()  # remove spaces
  assert v, f"missing '{k}' value"
  return k, smart_value(v)


def smart_value(v):
  """Convert a string to an underlying type such as int, float, bool, etc."""
  v_lower = v.lower()
  if v_lower == 'none':
    return None
  elif v_lower == 'true':
    return True
  elif v_lower == 'false':
    return False
  else:
    with contextlib.suppress(Exception):
      return eval(v)
    # deepcode ignore PythonDeadCode: <currently in planning phase>
    return v


def merge_equals_args(args: List[str]) -> List[str]:
  """
    Merges arguments around isolated '=' args in a list of strings. The function considers cases where the first
    argument ends with '=' or the second starts with '=', as well as when the middle one is an equals sign.

    Args:
        args (List[str]): A list of strings where each element is an argument.

    Returns:
        List[str]: A list of strings where the arguments around isolated '=' are merged.
    """
  new_args = []
  for i, arg in enumerate(args):
    if arg == '=' and 0 < i < len(args) - 1:  # merge ['arg', '=', 'val']
      new_args[-1] += f'={args[i + 1]}'
      del args[i + 1]
    elif arg.endswith('=') and i < len(args) - 1 and '=' not in args[i + 1]:  # merge ['arg=', 'val']
      new_args.append(f'{arg}{args[i + 1]}')
      del args[i + 1]
    elif arg.startswith('=') and i > 0:  # merge ['arg', '=val']
      new_args[-1] += arg
    else:
      new_args.append(arg)
  return new_args


def check_dict_alignment(base: Dict, custom: Dict, e=None):
  """
    This function checks for any mismatched keys between a custom configuration list and a base configuration list. If
    any mismatched keys are found, the function prints out similar keys from the base list and exits the program.

    Args:
        custom (dict): a dictionary of custom configuration options
        base (dict): a dictionary of base configuration options
        e (Error, optional): An optional error that is passed by the calling function.
    """
  base_keys, custom_keys = (set(x.keys()) for x in (base, custom))
  mismatched = [k for k in custom_keys if k not in base_keys]
  if mismatched:
    from difflib import get_close_matches

    string = ''
    for x in mismatched:
      matches = get_close_matches(x, base_keys)  # key list
      matches = [f'{k}={base[k]}' if base.get(k) is not None else k for k in matches]
      match_str = f'Similar arguments are i.e. {matches}.' if matches else ''
      string += f"'{colorstr('red', 'bold', x)}' is not a valid Netzwerx argument. {match_str}\n"
    raise SyntaxError(string + CLI_HELP_MSG) from e


def entrypoint(debug=''):
  """
    This function is the netzwerx package entrypoint, it's responsible for parsing the command line arguments passed
    to the package.

    This function allows for:
    - change mac address `maddr`

    It uses the package's default cfg and initializes it using the passed overrides.
    Then it calls the CLI function with the composed cfg
  """
  args = (debug.split(' ') if debug else sys.argv)[1:]
  if not args:  # no arguments passed
    LOGGER.info(CLI_HELP_MSG)
    return

  special = {
      'help': lambda: LOGGER.info(CLI_HELP_MSG),
      'version': lambda: LOGGER.info(__version__),
      'args': lambda: LOGGER.info(CLI_ARGS)}

  full_args_dict = {**{k: None for k in MODES}, **special}

  # Define common misuses of special commands, i.e. -h, -help, --help
  special.update({k[0]: v for k, v in special.items()})  # singular
  special.update({k[:-1]: v for k, v in special.items() if len(k) > 1 and k.endswith('s')})  # singular
  special = {**special, **{f'-{k}': v for k, v in special.items()}, **{f'--{k}': v for k, v in special.items()}}

  current_operations = {"mode": str, "args": {}}
  for a in merge_equals_args(args):  # merge spaces around '=' sign
    if a.startswith('--'):
      LOGGER.warning(f"WARNING ⚠️ '{a}' does not require leading dashes '--', updating to '{a[2:]}'.")
      a = a[2:]
    if a.endswith(','):
      LOGGER.warning(f"WARNING ⚠️ '{a}' does not require trailing comma ',', updating to '{a[:-1]}'.")
      a = a[:-1]
    if '=' in a:
      try:
        k, v = parse_key_value_pair(a)

        if current_operations["mode"] in MODES and v is not None:
          current_operations["args"].update({k: v})
          LOGGER.info(f'{k.capitalize()}: {v}')
        else:
          LOGGER.info("Please select proper mode")
          LOGGER.info(CLI_HELP_MSG)
      except (NameError, SyntaxError, ValueError, AssertionError) as e:
        check_dict_alignment(full_args_dict, {a: ''}, e)

    elif a in MODES:
      if a == "maddr":
        current_operations["mode"] = "maddr"

      if a == "nscan":
        current_operations["mode"] = "nscan"

    elif a.lower() in special:
      special[a.lower()]()
      return
    else:
      check_dict_alignment(full_args_dict, {a: ''})

  return select_mode_fn(current_operations["mode"], current_operations["args"])


if __name__ == '__main__':
  # Example: entrypoint(debug='netzwerx maddr')
  entrypoint(debug='')

import datetime
import fnmatch
import os
import platform
import re
import sys

from setuptools import Command
from setuptools import find_namespace_packages
from setuptools import setup
from setuptools.command.install import install as InstallCommandBase
from setuptools.dist import Distribution
from netzwerx import __version__

here = os.path.abspath(os.path.dirname(__file__))

with codecs.open(os.path.join(here, "Readme.md"), encoding="utf-8") as fh:
  long_description = "\n" + fh.read()

PROJECT_NAME = "netzwerx"
_VERSION = __version__
DESCRIPTION = 'A network module'
LONG_DESCRIPTION = 'A package that allows to [change mac address].'


class BinaryDistribution(Distribution):

  def has_ext_modules(self):
    return True


class InstallCommand(InstallCommandBase):
  """Override the dir where the headers go."""

  def finalize_options(self):
    ret = InstallCommandBase.finalize_options(self)  # pylint: disable=assignment-from-no-return
    self.install_headers = os.path.join(self.install_platlib, 'netzwerx', 'include')
    self.install_lib = self.install_platlib
    return ret


class InstallHeaders(Command):
  """Override how headers are copied.

  The install_headers that comes with setuptools copies all files to
  the same directory. But we need the files to be in a specific directory
  hierarchy for -I <include_dir> to work correctly.
  """
  description = 'install C/C++ header files'

  user_options = [
      ('install-dir=', 'd', 'directory to install header files to'),
      ('force', 'f', 'force installation (overwrite existing files)'), ]

  boolean_options = ['force']

  def initialize_options(self):
    self.install_dir = None
    self.force = 0
    self.outfiles = []

  def finalize_options(self):
    self.set_undefined_options('install', ('install_headers', 'install_dir'), ('force', 'force'))

  def mkdir_and_copy_file(self, header):
    install_dir = os.path.join(self.install_dir, os.path.dirname(header))

    # TODO(Adam-Al-Rahman): Add all the external headers
    external_header_locations = {
        '/external/com_google_absl': '', }

    for location in external_header_locations:
      if location in install_dir:
        extra_dir = install_dir.replace(location, external_header_locations[location])
        if not os.path.exists(extra_dir):
          self.mkpath(extra_dir)
        self.copy_file(header, extra_dir)

    if not os.path.exists(install_dir):
      self.mkpath(install_dir)
    return self.copy_file(header, install_dir)

  def run(self):
    hdrs = self.distribution.headers
    if not hdrs:
      return

    self.mkpath(self.install_dir)
    for header in hdrs:
      (out, _) = self.mkdir_and_copy_file(header)
      self.outfiles.append(out)

  def get_inputs(self):
    return self.distribution.headers or []

  def get_outputs(self):
    return self.outfiles


def find_files(pattern, root):
  """Return all the files matching pattern below root dir."""
  for dirpath, _, files in os.walk(root):
    for filename in fnmatch.filter(files, pattern):
      yield os.path.join(dirpath, filename)


so_lib_paths = [i for i in os.listdir('.') if os.path.isdir(i) and fnmatch.fnmatch(i, '_solib_*')]

matches = []
for path in so_lib_paths:
  matches.extend(['./' + x for x in find_files('*', path) if '.py' not in x])

# TODO(Adam-Al-Rahman): Add all the extensions
EXTENSION_NAME = []
if os.name == 'nt':  # Windows NT architecture
  EXTENSION_NAME.append('frontend/mac_address/_pywrap_mac_address.pyd')
else:
  EXTENSION_NAME.append('frontend/mac_address/_pywrap_mac_address.so')

# TODO(Adam-Al-Rahman): Setup the collaborator_build_dependent_options
collabrator_build_dependent_options = {
    'cmdclass': {
        'install_headers': InstallHeaders,
        'install': InstallCommand, },
    'distclass': BinaryDistribution,
    'entry_points': {
        'console_scripts': ["netzwerx = netzwerx.cfg:entrypoint", "nwx = netzwerx.cfg:entrypoint"], },
    'headers': headers,
    'include_package_data': True,
    'packages': find_namespace_packages(),
    'package_data': {
        'netzwerx': EXTENSION_NAME + matches, }, }

# Setting up
setup(
    name=PROJECT_NAME,
    license='Apache 2.0',
    license_file=LICENSE,
    version=_VERSION.replace('-', ''),
    author="Adam-Al-Rahman (https://atiq-ur-rehaman.netlify.app)",
    author_email="<quodscientialegis@gmail.com>",
    description=DESCRIPTION,
    long_description_content_type="text/markdown",
    long_description=long_description,
    install_requires=["rich"],
    keywords=['python', 'network', 'ethical hacking', 'cyber security', 'mac address'],
    classifiers=sorted([
        "Development Status :: 1 - Planning",
        "Intended Audience :: Developers",
        "Programming Language :: Python :: 3",
        "Operating System :: Unix",
        # "Operating System :: MacOS :: MacOS X",
        "Operating System :: Microsoft :: Windows",
        'Topic :: Security/Engineering',
        'Topic :: Security/Engineering :: Cyber Security',
        'Topic :: Security/Engineering :: Ethical Hacking',
        'Topic :: Software Development',
        'Topic :: Software Development :: Libraries',
        'Topic :: Software Development :: Libraries :: Python Modules', ]),
    **collaborator_build_dependent_options)

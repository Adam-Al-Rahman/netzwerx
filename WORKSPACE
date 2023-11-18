workspace(name = "org_netzwerx")

# Initialize the Netzwerx repo and all its dependencies.

# The cascade of load() statements and nw_workspace?() calls works around the
# restriction that load() statements need to be at the top of .bzl files.
# E.g. we can not retrieve a new repository with http_archive and then load()
# a macro from that repository in the same file.

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

http_archive(
    name = "bazel_skylib",
    sha256 = "74d544d96f4a5bb630d465ca8bbcfe231e3594e5aae57e1edbf17a6eb3ca2506",
    urls = ["https://github.com/bazelbuild/bazel-skylib/releases/download/1.3.0/bazel-skylib-1.3.0.tar.gz"],
)

http_archive(
    name = "rules_python",
    sha256 = "9d04041ac92a0985e344235f5d946f71ac543f1b1565f2cdbc9a2aaee8adf55b",
    strip_prefix = "rules_python-0.26.0",
    url = "https://github.com/bazelbuild/rules_python/releases/download/0.26.0/rules_python-0.26.0.tar.gz",
)

load("@rules_python//python:repositories.bzl", "py_repositories", "python_register_toolchains")

py_repositories()

python_register_toolchains(
    name = "python",
    python_version = "3.11",
)

load("@rules_python//python:pip.bzl", "pip_parse")
load("@python//:defs.bzl", "interpreter")

pip_parse(
    name = "py_requirements",
    python_interpreter_target = interpreter,
    requirements = ":requirements.txt",
)

load("@//netzwerx:workspace0.bzl", "nw_workspace0")

nw_workspace0()

load("@//netzwerx:workspace1.bzl", "nw_workspace1")

nw_workspace1()

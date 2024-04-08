""" Netzwerx workspace initialization. Consult the WORKSPACE on how to use it."""

# Import external repository rules.
load("@hedron_compile_commands//:workspace_setup.bzl", "hedron_compile_commands_setup")
load("@pybind11_bazel//:python_configure.bzl", "python_configure")
load("@python//:defs.bzl", "interpreter")
load("//third_party:repo.bzl", "nw_http_archive")

def _nw_repositories():
    """All external dependencies for NW builds."""
    nw_http_archive(
        name = "pybind11",
        url = "https://github.com/pybind/pybind11/archive/v2.10.4.tar.gz",
        sha256 = "832e2f309c57da9c1e6d4542dedd34b24e4192ecb4d62f6f4866a737454c9970",
        strip_prefix = "pybind11-2.10.4",
        build_file = "//third_party:pybind11.BUILD",
    )

    nw_http_archive(
        name = "com_google_googletest",
        sha256 = "8ad598c73ad796e0d8280b082cebd82a630d73e73cd3c70057938a6501bba5d7",
        strip_prefix = "googletest-1.14.0",
        url = "https://github.com/google/googletest/archive/refs/tags/v1.14.0.tar.gz",
    )

    nw_http_archive(
        name = "com_googlesource_code_re2",
        sha256 = "ef516fb84824a597c4d5d0d6d330daedb18363b5a99eda87d027e6bdd9cba299",
        strip_prefix = "re2-03da4fc0857c285e3a26782f6bc8931c4c950df4",
        build_file = "//third_party/systemlibs:re2.BUILD",
        url = "https://github.com/google/re2/archive/03da4fc0857c285e3a26782f6bc8931c4c950df4.tar.gz",
    )

def _nw_toolchains():
    """All toolchain required for NW"""

    python_configure(
        name = "local_config_python",
        python_interpreter_target = interpreter,
    )

def workspace():
    """ workspace of the netzwerx folders """
    hedron_compile_commands_setup()

    # Import all external repo
    _nw_repositories()

    # NW toolchains
    _nw_toolchains()

# Alias so it can be loaded without assigning to a different symbol to prevent
# shadowing previous loads and trigger a buildifier warning.
nw_workspace1 = workspace

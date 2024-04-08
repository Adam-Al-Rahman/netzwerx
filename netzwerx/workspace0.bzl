""" Netzwerx workspace initialization. Consult the WORKSPACE on how to use it."""

# Import third party config rules
load("//third_party:repo.bzl", "nw_http_archive")

# Import third party repository rules.
load("//third_party/absl:workspace.bzl", absl = "repo")
load("//third_party/hedron:workspace.bzl", hedron = "repo")
load("//third_party/pybind11_abseil:workspace.bzl", pybind11_abseil = "repo")
load("//third_party/pybind11_bazel:workspace.bzl", pybind11_bazel = "repo")
load("//third_party/subprocess:workspace.bzl", subprocess = "repo")

def _initialize_third_party():
    """ Load third party repositories.  See above load() statements. """
    absl()
    hedron()
    pybind11_abseil()
    pybind11_bazel()
    subprocess()

# Define all external repositories required by Netzwerx
def _nw_repositories():
    """All external dependencies for RR builds."""

    # To update any of the dependencies below:
    # a) update URL and strip_prefix to the new git commit hash
    # b) get the sha256 hash of the commit by running:
    #    curl -L <url> | sha256sum
    # and update the sha256 with the result.

    nw_http_archive(
        name = "googletest",
        sha256 = "bc1cc26d1120f5a7e9eb450751c0b24160734e46a02823a573f3c6b6c0a574a7",
        strip_prefix = "googletest-e2c06aa2497e330bab1c1a03d02f7c5096eb5b0b",
        url = "https://github.com/google/googletest/archive/e2c06aa2497e330bab1c1a03d02f7c5096eb5b0b.zip",
    )

    # gflags is a dependency of glog.
    nw_http_archive(
        name = "com_github_gflags_gflags",
        sha256 = "34af2f15cf7367513b352bdcd2493ab14ce43692d2dcd9dfc499492966c64dcf",
        strip_prefix = "gflags-2.2.2",
        url = "https://github.com/gflags/gflags/archive/v2.2.2.tar.gz",
    )

    nw_http_archive(
        name = "glog",
        sha256 = "f28359aeba12f30d73d9e4711ef356dc842886968112162bc73002645139c39c",
        strip_prefix = "glog-0.4.0",
        url = "https://github.com/google/glog/archive/refs/tags/v0.4.0.tar.gz",
    )

def workspace():
    """ workspace of the netzwerx folders """

    # Import third party repositories.
    _initialize_third_party()

    # Import all other repositories. This should happen before initializing
    # any external repositories, because those come with their own
    # dependencies. Those recursive dependencies will only be imported if they
    # don't already exist (at least if the external repository macros were
    # written according to common practice to query native.existing_rule()).
    _nw_repositories()

# Alias so it can be loaded without assigning to a different symbol to prevent
# shadowing previous loads and trigger a buildifier warning.
nw_workspace0 = workspace

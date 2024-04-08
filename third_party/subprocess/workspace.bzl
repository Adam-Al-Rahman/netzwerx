"""Provides the repository macro to import cpp-subprocess."""

load("//third_party:repo.bzl", "nw_http_archive")

def repo():
    """Imports cpp-subprocess."""

    SUBPROCESS_COMMIT = "d809374661b316521f36dd56f4426c043f83b279"
    SUBPROCESS_SHA256 = "3aeac8b509187dc160a27f586f71b99865c0daa2064ca379f662ab282359c45e"

    nw_http_archive(
        name = "arun11299_subprocess",
        sha256 = SUBPROCESS_SHA256,
        add_prefix = "subprocess",
        build_file = "//third_party/subprocess:arun11299_subprocess.BUILD",
        strip_prefix = "cpp-subprocess-{commit}".format(commit = SUBPROCESS_COMMIT),
        url = "https://github.com/arun11299/cpp-subprocess/archive/{commit}.tar.gz".format(commit = SUBPROCESS_COMMIT),
    )

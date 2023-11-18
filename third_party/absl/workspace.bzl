"""Provides the repository macro to import absl."""

load("//third_party:repo.bzl", "nw_http_archive")

def repo():
    """Imports absl."""

    ABSL_COMMIT = "3bd86026c93da5a40006fd53403dff9d5f5e30e3"
    ABSL_SHA256 = "67ccfb6597a22062e0233c387858d9ad00b711c8ffa10a85cce07537abbd61fa"

    SKYLIB_SHA256 = "f7be3474d42aae265405a592bb7da8e171919d74c16f082a5457840f06054728"

    # bazel_skylib is dependency of abseil
    nw_http_archive(
        name = "bazel_skylib",
        url = "https://github.com/bazelbuild/bazel-skylib/releases/download/1.2.1/bazel-skylib-1.2.1.tar.gz",
        sha256 = SKYLIB_SHA256,
    )

    nw_http_archive(
        name = "com_google_absl",
        sha256 = ABSL_SHA256,
        build_file = "//third_party/absl:com_google_absl.BUILD",
        strip_prefix = "abseil-cpp-{commit}".format(commit = ABSL_COMMIT),
        url = "https://github.com/abseil/abseil-cpp/archive/{commit}.tar.gz".format(commit = ABSL_COMMIT),
    )

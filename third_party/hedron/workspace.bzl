"""Provides the repository macro to import Eigen."""

load("//third_party:repo.bzl", "nw_http_archive")

def repo():
    """Imports Hedron's Compile Commands Extractor for Bazel."""

    HEDRON_COMMIT = "12440540f4461cac006a357257d698aed74a2423"
    HEDRON_SHA256 = "2f6c129770dbc7a0bb53057b8a225ee1eb275bf27e05504b683f8d96d2c877c9"

    # https://github.com/hedronvision/bazel-compile-commands-extractor
    nw_http_archive(
        name = "hedron_compile_commands",
        url = "https://github.com/hedronvision/bazel-compile-commands-extractor/archive/{commit}.tar.gz".format(commit = HEDRON_COMMIT),
        strip_prefix = "bazel-compile-commands-extractor-{commit}".format(commit = HEDRON_COMMIT),
        sha256 = HEDRON_SHA256,
    )

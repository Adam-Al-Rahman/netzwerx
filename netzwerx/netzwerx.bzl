"""
Define a bazel macro that creates for netzwerx.
Default build rules and function for netzwerx.
"""

load("//netzwerx/core/default:rules_cc.bzl", "cc_test")

# netzwerx cc_test macro
def nw_cc_test(
        name,
        srcs,
        deps,
        data = [],
        suffix = "",
        **kwargs):
    cc_test(
        name = "%s%s" % (name, suffix),
        srcs = srcs,
        deps = deps,
        data = data,
        **kwargs
    )

# netzwerx filegroup function
def nw_filegroup(**kwargs):
    native.filegroup(**kwargs)

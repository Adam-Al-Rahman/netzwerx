"""
Define a bazel macro that creates for netzwerx.
Default build rules and function for netzwerx.
"""

load("@pybind11_bazel//:build_defs.bzl", "pybind_extension")
load("@rules_python//python:defs.bzl", "py_library")
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

# netzwerx pybind extension
# buildozer: disable=function-docstring-args
def nw_pybind_extension(
        name,
        srcs,
        deps = [],
        pytype_srcs = [],
        pytype_deps = [],
        srcs_version = "PY3",
        visibility = None):
    p = name.rfind("/")
    if p == -1:
        sname = name
        prefix = ""
    else:
        sname = name[p + 1:]
        prefix = name[:p + 1]

    so_file = "%s%s.so" % (prefix, sname)

    # filegroup_name = "%s_filegroup" % name
    pyd_file = "%s%s.pyd" % (prefix, sname)

    pybind_extension(
        name = name,
        srcs = srcs,
        deps = deps,
        visibility = visibility,
    )

    py_library(
        name = name,
        data = select({
            "@platforms//os:windows": [pyd_file],
            "//conditions:default": [so_file],
        }) + pytype_srcs,
        deps = pytype_deps,
        srcs_version = srcs_version,
        visibility = visibility,
    )

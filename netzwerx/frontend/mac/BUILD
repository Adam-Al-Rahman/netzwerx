load("@rules_python//python:defs.bzl", "py_binary")
load("//netzwerx:netzwerx.bzl", "nw_pybind_extension")

nw_pybind_extension(
    name = "_pywrap_mac_address",
    srcs = ["mac_address_wrapper.cc"],
    deps = [
        "//netzwerx/backend:mac_address",
    ],
    pytype_srcs = ["_pywrap_mac_address.pyi"],
    visibility = ["//visibility:public"],
)

# py_library(
#     name = "py_mac_address",
#     srcs = [
#         "mac_address.py",
#     ],
#     data = select({
#         "@platforms//os:linux": [
#             "_pywrap_mac_address.so",
#         ],
#         "@platforms//os:windows": [
#             "_pywrap_mac_address.dll",
#         ],
#         "//conditions:default": [
#             "_pywrap_mac_address.so",
#         ],
#     }) + ["_pywrap_mac_address.pyi"],
#     srcs_version = "PY3",
# )

py_binary(
    name = "py_mac_address",
    srcs = [
        "mac_address.py",
    ],
    deps = [":_pywrap_mac_address"],
    main = "mac_address.py",
    srcs_version = "PY3",
)

load("@pybind11_bazel//:build_defs.bzl", "pybind_extension")

pybind_extension(
    name="_pywrap_mac_address",
    srcs=["mac_address_wrapper.cc"],
    visibility=["//visibility:public"],
    deps=[
        "//netzwerx/backend:mac_address", ],
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
    name="py_mac_address",
    srcs=[
        "mac_address.py", ],
    data=select({
        "@platforms//os:linux": ["_pywrap_mac_address.so", ],
        "@platforms//os:windows": ["_pywrap_mac_address.pyd", ],
        "//conditions:default": ["_pywrap_mac_address.so", ], }) + ["_pywrap_mac_address.pyi"],
    main="mac_address.py",
    srcs_version="PY3",
)

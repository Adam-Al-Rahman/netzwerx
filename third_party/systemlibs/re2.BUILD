load("@rules_cc//cc:defs.bzl", "cc_library")

licenses(["notice"])  # BSD/MIT-like license

filegroup(
    name = "LICENSE",
    visibility = ["//visibility:public"],
)

cc_library(
    name = "re2",
    linkopts = ["-lre2"],  # sudo apt-get install libre2-dev
    visibility = ["//visibility:public"],
)

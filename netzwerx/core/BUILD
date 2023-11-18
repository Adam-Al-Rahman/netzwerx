# load("//netzwerx:netzwerx.bzl", "nw_cc_test")
load("//netzwerx/core/default:rules_cc.bzl", "cc_library")

cc_library(
    name = "core_platform",
    srcs = [
        "platform/platform.cc",
    ],
    hdrs = [
        "platform/platform.h",
    ],
    visibility = ["//netzwerx/backend:__subpackages__"],
    deps = [
        "@com_google_absl//absl/strings",
    ],
)

cc_library(
    name = "super_user",
    srcs = [
        "privilege/super_user.cc",
    ],
    hdrs = [
        "privilege/super_user.h",
    ],
    visibility = ["//netzwerx/backend:__subpackages__"],
)

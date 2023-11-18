load("//netzwerx/core/default:rules_cc.bzl", "cc_library", "cc_test")

cc_library(
    name = "mac_address_utils",
    srcs = ["utils/mac_address_utils.cc"],
    hdrs = ["utils/mac_address_utils.h"],
    visibility = ["//visibility:public"],
    deps = [
        "//netzwerx/core:core_platform",
    ],
)

cc_library(
    name = "mac_address",
    srcs = ["mac_address/mac_address.cc"],
    hdrs = ["mac_address/mac_address.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":mac_address_utils",
        "//netzwerx/core:core_platform",
        "//netzwerx/core:super_user",
        # "@arun11299_cpp_subprocess//:subprocess.hpp" # TODO(Adam-Al-Rahman): switch with custom method
    ],
)

cc_library(
    name = "mac_address_mock",
    hdrs = [
        "mac_address/mac_address_mock.h",
    ],
    deps = [
        "//netzwerx/core:core_platform",
        "@com_google_googletest//:gtest_main",
    ],
    visibility = ["//visibility:public"],
)


cc_test(
    name = "mac_address_test",
    srcs = ["mac_address/mac_address_test.cc"],
    deps = [
        ":mac_address_mock",
        "@com_google_googletest//:gtest_main",
    ],
    size = "small",
    timeout = "short",
)

cc_test(
    name = "mac_address_utils_test",
    srcs = ["utils/mac_address_utils_test.cc"],
    deps = [
        ":mac_address_utils",
        "@com_google_googletest//:gtest_main",
    ],
    size = "small",
    timeout = "short",
)

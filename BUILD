load("@rules_cc//cc:cc_binary.bzl", "cc_binary")
load("@rules_cc//cc:cc_library.bzl", "cc_library")

cc_library(
    name = "cpu_components",
    srcs = [
        "src/Calculator.cpp",
        "src/Parser.cpp",
        "src/Printer.cpp",
    ],
    hdrs = [
        "include/Calculator.h",
        "include/Parser.h",
        "include/Printer.h",
    ],
    includes = ["include"],
)

cc_binary(
    name = "cpu_load",
    srcs = ["main.cpp"],
    deps = [":cpu_components"],
)

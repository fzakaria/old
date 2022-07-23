# https://stackoverflow.com/questions/32845940/symbols-from-static-cc-library-dependency-in-so-missing
cc_binary(
    name = "libold.so",
    srcs = [
        "main.cpp",
    ],
    # For LD_AUDIT, we need to be sure that all symbols are defiend
    linkopts = [
        "-shared",
        "-Wl,-z defs",
    ],
    deps = [
        "//src/config",
        "//src/strategy:strategies",
        "@com_github_google_glog//:glog",
        "@tomlplusplus//:main",
    ],
)

cc_binary(
    name = "libold-audit.so",
    srcs = [
        "audit.cpp",
    ],
    # For LD_AUDIT, we need to be sure that all symbols are defiend
    linkopts = [
        "-shared",
        "-Wl,-z defs",
    ],
    deps = [
        "//src/config",
        "@com_github_google_glog//:glog",
    ],
)

cc_binary(
    name = "libold-histogram.so",
    srcs = [
        "histogram.cpp",
    ],
    # For LD_AUDIT, we need to be sure that all symbols are defiend
    linkopts = [
        "-shared",
        "-Wl,-z defs",
    ],
    deps = [
        "@com_github_google_glog//:glog",
        "@lief",
    ],
)

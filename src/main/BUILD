# https://stackoverflow.com/questions/32845940/symbols-from-static-cc-library-dependency-in-so-missing
cc_binary(
    name = "libold.so",
    srcs = [
        "main.cpp",
    ],
    linkopts = ["-shared"],
    deps = [
        "//src/strategy:strategies",
        "//src/config:config",
        "@com_github_google_glog//:glog",
        "@tomlplusplus//:main",
    ],
)

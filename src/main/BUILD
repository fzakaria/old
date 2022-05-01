# https://stackoverflow.com/questions/32845940/symbols-from-static-cc-library-dependency-in-so-missing
cc_binary(
    name = "libold.so",
    srcs = [
        "main.cpp",
    ],
    deps = ["@tomlplusplus//:main", "//src/strategy:strategies"],
    linkopts = ["-shared"]
)
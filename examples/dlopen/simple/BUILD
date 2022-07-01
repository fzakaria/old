# https://stackoverflow.com/questions/32845940/symbols-from-static-cc-library-dependency-in-so-missing
cc_binary(
    name = "libhello.so",
    srcs = [
        "hello.cpp",
    ],
    linkopts = ["-shared"],
    deps = [
    ],
)

cc_binary(
    name = "main",
    srcs = [
        "main.cpp",
    ],
    linkopts = ["-ldl", "-Wl,-rpath,$$ORIGIN"],
)

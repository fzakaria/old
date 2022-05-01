load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

http_archive(
    name = "tomlplusplus",
    url = "https://github.com/marzer/tomlplusplus/archive/refs/tags/v3.0.1.zip",
    sha256 = "49b7e619a4f545c2e7fbeda36853244c36cbf85818d92953fddbec4799359c27",
    build_file = "@//third_party:tomlplusplus.BUILD",
    strip_prefix = "tomlplusplus-3.0.1",
)

http_archive(
    name = "com_github_gflags_gflags",
    sha256 = "34af2f15cf7367513b352bdcd2493ab14ce43692d2dcd9dfc499492966c64dcf",
    strip_prefix = "gflags-2.2.2",
    urls = ["https://github.com/gflags/gflags/archive/v2.2.2.tar.gz"],
)

http_archive(
    name = "com_github_google_glog",
    sha256 = "122fb6b712808ef43fbf80f75c52a21c9760683dae470154f02bddfc61135022",
    strip_prefix = "glog-0.6.0",
    urls = ["https://github.com/google/glog/archive/v0.6.0.zip"],
)
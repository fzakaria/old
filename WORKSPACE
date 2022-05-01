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

rules_python_version = "ae7a2677b3003b13d45bc9bfc25f1425bed5b407" # 0.8 release

http_archive(
    name = "rules_python",
    sha256 = "f1c3069679395ac1c1104f28a166f06167d30d41bdb1797d154d80b511780d2e",
    strip_prefix = "rules_python-{}".format(rules_python_version),
    url = "https://github.com/bazelbuild/rules_python/archive/{}.zip".format(rules_python_version),
)

http_archive(
  name = "com_google_googletest",
  urls = ["https://github.com/google/googletest/archive/8ded48c37be09d8cc3665af1b414c5d53c0862e7.zip"],
  strip_prefix = "googletest-8ded48c37be09d8cc3665af1b414c5d53c0862e7",
)
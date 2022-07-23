load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

# Hedron's Compile Commands Extractor for Bazel
# https://github.com/hedronvision/bazel-compile-commands-extractor
http_archive(
    name = "hedron_compile_commands",
    sha256 = "4a1712a3319ddb97c1b215e9927787344b4fddaafc4ea166cf3095a767b02986",
    strip_prefix = "bazel-compile-commands-extractor-a36dd55381f5bc452aa9f5392a8f8472a6270450",

    # Replace the commit hash in both places (below) with the latest, rather than using the stale one here.
    # Even better, set up Renovate and let it do the work for you (see "Suggestion: Updates" in the README).
    url = "https://github.com/hedronvision/bazel-compile-commands-extractor/archive/a36dd55381f5bc452aa9f5392a8f8472a6270450.tar.gz",
)

load("@hedron_compile_commands//:workspace_setup.bzl", "hedron_compile_commands_setup")

hedron_compile_commands_setup()

http_archive(
    name = "sqlite",
    build_file = "//third_party:sqlite.BUILD",
    sha256 = "f2ce17bca51b376de4fcb9d0dc174c52f472a34c29aa1ccc774f27467cc63ed7",
    strip_prefix = "sqlite-amalgamation-3390100",
    url = "https://www.sqlite.org/2022/sqlite-amalgamation-3390100.zip",
)

http_archive(
    name = "tomlplusplus",
    build_file = "@//third_party:tomlplusplus.BUILD",
    sha256 = "49b7e619a4f545c2e7fbeda36853244c36cbf85818d92953fddbec4799359c27",
    strip_prefix = "tomlplusplus-3.0.1",
    url = "https://github.com/marzer/tomlplusplus/archive/refs/tags/v3.0.1.zip",
)

http_archive(
    name = "lief",
    build_file = "@//third_party:lief.BUILD",
    sha256 = "b1c30381a258d05a9812020e7391cfd92ef0043a2d16e4d179d2c788a263b9f7",
    strip_prefix = "LIEF-0.12.1-Linux-x86_64",
    url = "https://github.com/lief-project/LIEF/releases/download/0.12.1/LIEF-0.12.1-Linux-x86_64.tar.gz",
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

rules_python_version = "ae7a2677b3003b13d45bc9bfc25f1425bed5b407"  # 0.8 release

http_archive(
    name = "rules_python",
    sha256 = "f1c3069679395ac1c1104f28a166f06167d30d41bdb1797d154d80b511780d2e",
    strip_prefix = "rules_python-{}".format(rules_python_version),
    url = "https://github.com/bazelbuild/rules_python/archive/{}.zip".format(rules_python_version),
)

http_archive(
    name = "com_google_googletest",
    strip_prefix = "googletest-8ded48c37be09d8cc3665af1b414c5d53c0862e7",
    urls = ["https://github.com/google/googletest/archive/8ded48c37be09d8cc3665af1b414c5d53c0862e7.zip"],
)

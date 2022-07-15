
# Recommended compiler flags
# https://www.sqlite.org/compile.html
SQLITE_COPTS = [
    "-DSQLITE_DQS=0",
    "-DSQLITE_THREADSAFE=0",
    "-DSQLITE_DEFAULT_MEMSTATUS=0",
    "-DSQLITE_DEFAULT_WAL_SYNCHRONOUS=1",
    "-DSQLITE_LIKE_DOESNT_MATCH_BLOBS=1",
    "-DSQLITE_MAX_EXPR_DEPTH=0",
    "-DSQLITE_OMIT_DEPRECATED",
    "-DSQLITE_OMIT_PROGRESS_CALLBACK",
    "-DSQLITE_OMIT_SHARED_CACHE",
    "-DSQLITE_USE_ALLOCA",
    "-DSQLITE_OMIT_AUTOINIT",
]

cc_library(
    name = "sqlite",
    srcs = ["sqlite3.c"],
    hdrs = [
        "sqlite3.h",
        "sqlite3ext.h",
    ],
    copts = SQLITE_COPTS,
    defines = ["SQLITE_OMIT_DEPRECATED"],
    linkopts = ["-ldl", "-lpthread"],
    visibility = ["//visibility:public"],
)
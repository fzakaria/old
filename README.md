# old

![main brabcg](https://github.com/fzakaria/old/actions/workflows/actions.yml/badge.svg)

> The Other Dynamic Linker

The algorithm with which dependencies are found and selected have not changed since it's introduction in [Multics](https://en.wikipedia.org/wiki/Multics) and codifies in [SystemV ABI](http://www.sco.com/developers/devspecs/gabi41.pdf),

The current knobs offered by dynamic linkers are quite coarse by only allowing single lists that affect the resolution for any dependency such as _LD_LIBRARY_PATH_ or _RUN_PATH_.

This project is an exploration into _alternative_ resolution strategies for dynamic linking by leveraging [glibc runtime linking audit API](https://man7.org/linux/man-pages/man7/rtld-audit.7.html).

Let's take a look a minimal example of doing a _key-value_ resolution strategy for the needed dependencies.

We create our _TOML_ file with the necessary information.
```toml
strategy = "key_value"

[key_value]
# if strict mode is enabled, then all libraries must be included
strict = true
# these need to be quoted since they have periods
"libruby-3.0.so.3.0" = "/lib/x86_64-linux-gnu/libruby-3.0.so.3.0"
"libc.so.6" = "/lib/x86_64-linux-gnu/libc.so.6"
"libpthread.so.0" = "/lib/x86_64-linux-gnu/libpthread.so.0"
"librt.so.1" = "/lib/x86_64-linux-gnu/librt.so.1"
"libgmp.so.10" = "/lib/x86_64-linux-gnu/libgmp.so.10"
"libdl.so.2" = "/lib/x86_64-linux-gnu/libdl.so.2"
"libcrypt.so.1" = "/lib/x86_64-linux-gnu/libcrypt.so.1"
"libm.so.6" = "/lib/x86_64-linux-gnu/libm.so.6"
"libz.so.1" = "/lib/x86_64-linux-gnu/libz.so.1"
```

We can now invoke any binary that has glibc dynamic linker in the interpreter section of the binary and specify _LD_AUDIT_ environment variable.

```console
$ OLDAUDIT_CONFIG=./example.toml LD_AUDIT=./bazel-bin/src/main/libold.so ruby --help | head

I20220506 01:30:34.472784 1547568 main.cpp:56] Taking control of the linking search....
I20220506 01:30:34.473285 1547568 key_value.cpp:17] replacing [libruby-3.0.so.3.0] with ['/lib/x86_64-linux-gnu/libruby-3.0.so.3.0']
I20220506 01:30:34.473435 1547568 key_value.cpp:17] replacing [libc.so.6] with ['/lib/x86_64-linux-gnu/libc.so.6']
I20220506 01:30:34.473569 1547568 key_value.cpp:17] replacing [libz.so.1] with ['/lib/x86_64-linux-gnu/libz.so.1']
I20220506 01:30:34.473695 1547568 key_value.cpp:17] replacing [libpthread.so.0] with ['/lib/x86_64-linux-gnu/libpthread.so.0']
I20220506 01:30:34.473809 1547568 key_value.cpp:17] replacing [librt.so.1] with ['/lib/x86_64-linux-gnu/librt.so.1']
I20220506 01:30:34.473938 1547568 key_value.cpp:17] replacing [libgmp.so.10] with ['/lib/x86_64-linux-gnu/libgmp.so.10']
I20220506 01:30:34.474058 1547568 key_value.cpp:17] replacing [libdl.so.2] with ['/lib/x86_64-linux-gnu/libdl.so.2']
I20220506 01:30:34.474195 1547568 key_value.cpp:17] replacing [libcrypt.so.1] with ['/lib/x86_64-linux-gnu/libcrypt.so.1']
I20220506 01:30:34.474318 1547568 key_value.cpp:17] replacing [libm.so.6] with ['/lib/x86_64-linux-gnu/libm.so.6']
Usage: ruby [switches] [--] [programfile] [arguments]
  -0[octal]       specify record separator (\0, if no argument)
  -a              autosplit mode with -n or -p (splits $_ into $F)
  -c              check syntax only
  -Cdirectory     cd to directory before executing your script
  -d, --debug     set debugging flags (set $DEBUG to true)
  -e 'command'    one line of script. Several -e's allowed. Omit [programfile]
  -Eex[:in], --encoding=ex[:in]
                  specify the default external and internal character encodings
  -Fpattern       split() pattern for autosplit (-a)
```

> Right now the log messages are emitted to _stderr_ always.

If you want to explore other strategies take a look at [example.toml](example.toml).

## Development

This repository uses [bazel](https://docs.bazel.build/) as the build system; some familiarity is required.

Once you have bazel installed, building the shared object is done as follows.
```console
$ bazel build src/main:libold.so
```

You can then run the built binary with the example TOML file provided by setting the necessary environment variables.

```console
$ OLDAUDIT_CONFIG=./example.toml LD_AUDIT=./bazel-bin/src/main/libold.so ruby --help | head
```
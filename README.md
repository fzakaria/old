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

## Auditing

Oftentimes, binaries require more libraries than what is specifically required in the _DT_NEEDED_ section.
Let's use `ruby` as an example.

Here we can see what is required by the binary itself; there are 11 libraries.
```console
$ ldd $(which ruby)
	linux-vdso.so.1 (0x00007ffc5eb93000)
	/lib/x86_64-linux-gnu/libnss_cache.so.2 (0x00007f8b68788000)
	libruby-3.0.so.3.0 => /lib/x86_64-linux-gnu/libruby-3.0.so.3.0 (0x00007f8b683f3000)
	libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007f8b6821a000)
	libz.so.1 => /lib/x86_64-linux-gnu/libz.so.1 (0x00007f8b681fd000)
	libpthread.so.0 => /lib/x86_64-linux-gnu/libpthread.so.0 (0x00007f8b681dc000)
	librt.so.1 => /lib/x86_64-linux-gnu/librt.so.1 (0x00007f8b681d0000)
	libgmp.so.10 => /lib/x86_64-linux-gnu/libgmp.so.10 (0x00007f8b6814f000)
	libdl.so.2 => /lib/x86_64-linux-gnu/libdl.so.2 (0x00007f8b68149000)
	libcrypt.so.1 => /lib/x86_64-linux-gnu/libcrypt.so.1 (0x00007f8b6810e000)
	libm.so.6 => /lib/x86_64-linux-gnu/libm.so.6 (0x00007f8b67fca000)
	/lib64/ld-linux-x86-64.so.2 (0x00007f8b68796000)
```

We can run `ruby` using this _LD_AUDIT_ tool to capture all other dynamically needed libaries so that we can also
include them in our key-value map, therefore, speeding up builds.

```console
❯ LD_AUDIT=./bazel-bin/src/main/libold.so ruby -e "puts 'hi'"
.. prune for brevity ...
hi
I20220701 21:54:58.348860 1715429 main.cpp:18] Profiled the following libraries which were loaded
strategy = 'key_value'

[key_value]
'encdb.so' = '/usr/lib/x86_64-linux-gnu/ruby/3.0.0/enc/encdb.so'
'ld-linux-x86-64.so.2' = '/lib64/ld-linux-x86-64.so.2'
'libc.so.6' = '/lib/x86_64-linux-gnu/libc.so.6'
'libcrypt.so.1' = '/lib/x86_64-linux-gnu/libcrypt.so.1'
'libdl.so.2' = '/lib/x86_64-linux-gnu/libdl.so.2'
'libgmp.so.10' = '/lib/x86_64-linux-gnu/libgmp.so.10'
'libm.so.6' = '/lib/x86_64-linux-gnu/libm.so.6'
'libnss_cache.so.2' = '/lib/x86_64-linux-gnu/libnss_cache.so.2'
'libpthread.so.0' = '/lib/x86_64-linux-gnu/libpthread.so.0'
'librt.so.1' = '/lib/x86_64-linux-gnu/librt.so.1'
'libruby-3.0.so.3.0' = '/lib/x86_64-linux-gnu/libruby-3.0.so.3.0'
'libz.so.1' = '/lib/x86_64-linux-gnu/libz.so.1'
'monitor.so' = '/usr/lib/x86_64-linux-gnu/ruby/3.0.0/monitor.so'
strict = false
'transdb.so' = '/usr/lib/x86_64-linux-gnu/ruby/3.0.0/enc/trans/transdb.so'
```

We now have a much more exhaustive list of libraries that were needed by the application which we can include.
The tool generously provides the required TOML file necessary for the configuration to use on subsequent runs.

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

### Nix

You can get all the needed dependencies via [Nix](https://nixos.org).
You can quickly just build the binary with Nix
```console
$ nix build .#old
$ ls result/bin
libold.so
```

You can also start up a Nix development shell and have all the dependencies provided.
```console
$ nix develop
nix develop
warning: Git tree '/usr/local/google/home/fmzakari/code/github.com/fzakaria/old' is dirty
$ which bazel
/nix/store/hzmmgvs3hknwwaxansd984c8gjkwyjgv-bazel-3.7.2/bin/bazel
$ which clang
/nix/store/8jl4ks1m051dafvl7lcdmwjsl63k5xck-clang-wrapper-7.1.0/bin/clang
```
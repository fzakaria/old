# old
The Other Dynamic Linker

```console
$ bazel build src/main:old

$ OLDAUDIT_CONFIG=./example.toml LD_AUDIT=./bazel-bin/src/main/libold.so ruby --help | head
```
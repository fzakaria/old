{ buildBazelPackage
, lib
, bazel
, clang
}:
buildBazelPackage {
  name = "old";
  # https://nix.dev/anti-patterns/language#reproducibility-referencing-top-level-directory-with
  src = builtins.path { path = ./.; name = "old-source"; };
  buildInputs = [ bazel clang ];
  bazelTarget = "src/main:libold.so";
  buildAttrs = {
    installPhase = ''
      mkdir -p $out/bin
      # Cannot use bazel info bazel-bi
      # since it points to the wrong directory
      cp ./bazel-bin/src/main/libold.so $out/bin
    '';
  };
  fetchAttrs = {
    sha256 = "sha256-MJ5cOm/ffgsmjdXgHyzfLVrr19kT7qGWBwfgTp2aHPA=";
  };
}

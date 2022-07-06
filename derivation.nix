{ stdenv
, lib
, bazel
}:
stdenv.mkDerivation {
  name = "old";
  # https://nix.dev/anti-patterns/language#reproducibility-referencing-top-level-directory-with
  builtins.path { path = ./.; name = "old-source"; };
  buildInputs = [ bazel, clang ];
  buildCommand = ''
    bazel build //...
    ls $out
  '';
}
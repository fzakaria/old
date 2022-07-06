{
  description = "The Other Dynamic Linker";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-21.11";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = import nixpkgs {
          inherit system;
          overlays = [ (import ./overlay.nix) ];
        };
      in
      {
        packages = {
          old = pkgs.old;
        };

        checks = {
        };

        defaultPackage = pkgs.old;

        devShell = pkgs.old;
 });
}

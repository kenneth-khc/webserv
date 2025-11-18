{
  description = "C++ devshell";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs =
    {
      self,
      nixpkgs,
      flake-utils,
      ...
    }:
    flake-utils.lib.eachDefaultSystem (
      system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
      in
      {
        devShells = with pkgs; {
          default = mkShell {
            packages = with pkgs; [
              clang
              gnumake
              gdb
              bear
              php
              python314
              inetutils
            ];
            hardeningDisable = [ "all" ];
          };
        };
      }
    );
}

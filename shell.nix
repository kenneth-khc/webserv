{ pkgs ? import <nixpkgs> {} }:
	pkgs.mkShell {
		nativeBuildInputs = with pkgs.buildPackages; [
			gnumake
			gcc
			gdb
			php
			python314
		];
	hardeningDisable = [ "all" ];
}
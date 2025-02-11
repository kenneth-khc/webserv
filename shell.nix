{ pkgs ? import <nixpkgs> {} }:
	pkgs.mkShell {
		nativeBuildInputs = with pkgs.buildPackages; [
			gnumake
			gcc
			gdb
		];
	hardeningDisable = [ "all" ];
}
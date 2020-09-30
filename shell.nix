{ pkgs ? import <nixpkgs> {} }:
  pkgs.clangStdenv.mkDerivation {
    name = "nelder-mead-shell";

    nativeBuildInputs = with pkgs; [
      gdb
      pkg-config
      cmake
      clang-analyzer
      clang-tools
      valgrind
    ];

    buildInputs = with pkgs; [
                    mesa
		    mesa_glu
                    qt5.full
                    qtcreator
		    eigen
                  ];
}

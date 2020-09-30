{ pkgs ? import <nixpkgs> {} }:
  pkgs.clangStdenv.mkDerivation {
    name = "nelder-mead-shell";

    nativeBuildInputs = with pkgs; [
      pkg-config
      cmake
      clang-analyzer
      clang-tools
      valgrind
    ];

    buildInputs = with pkgs; [
                    qt5.full
                    qtcreator
		    eigen
                  ];
}

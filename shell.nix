let pkgs = import (fetchTarball {
  #url = "https://github.com/NixOS/nixpkgs/archive/b4db68ff563895eea6aab4ff24fa04ef403dfe14.tar.gz";
  url = "https://github.com/NixOS/nixpkgs/archive/release-20.09.tar.gz";
}) {};
in
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
                    qt514.full
                    qtcreator
		    eigen
                  ];
}

with import <nixpkgs> {};

{guiOn ? false}:
clangStdenv.mkDerivation rec {
  pname = "nelder-mead";
  version = "0.0.1";
  src = ./.;

  nativeBuildInputs = [
    cmake
    pkg-config
  ];

  buildInputs = [ 
    mesa
    mesa_glu
    clang 
    qt514.qtbase
  ];

  installPhase = ''
    mkdir -p $out/bin
    cp app/nelder-mead $out/bin
  '';
}

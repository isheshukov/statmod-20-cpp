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
    eigen
    clang 
    qt514.full
  ];

  installPhase = ''
    mkdir -p $out
    cp app/nelder-mead $out
  '';

}

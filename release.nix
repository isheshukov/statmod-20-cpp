with import <nixpkgs> {};

{guiOn ? false}:
clangStdenv.mkDerivation rec {
  pname = "nelder-mead";
  version = "0.0.1";
  src = ./.;

  nativeBuildInputs = [
    cmake
  ];

  buildInputs = [ 
    eigen
    clang 
    qt5.full
  ];

  cmakeFlags = []
   ++ stdenv.lib.optional guiOn "-DUSE_GUI=1";

  installPhase = ''
    mkdir -p $out
    cp app/nelder-mead $out
  '';

}

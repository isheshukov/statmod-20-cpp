with import <nixpkgs> {};

{guiOn ? false}:
stdenv.mkDerivation rec {
  pname = "nelder-mead";
  version = "0.0.1";
  src = ./.;

  nativeBuildInputs = [
    cmake
  ];

  buildInputs = [ 
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

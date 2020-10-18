let
  # Pinning explicitly to 20.03.
  rev = "ad0d20345219790533ebe06571f82ed6b034db31";
  nixpkgs = fetchTarball "https://github.com/NixOS/nixpkgs/archive/${rev}.tar.gz";
  pkgs = import nixpkgs {};
in
pkgs.clangStdenv.mkDerivation rec {
  pname = "nelder-mead";
  version = "0.0.1";
  src = ./.;

  nativeBuildInputs = with pkgs; [
    cmake
    pkg-config
  ];

  buildInputs = with pkgs; [ 
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

let
  # Pinning explicitly to 20.03.
  rev = "ad0d20345219790533ebe06571f82ed6b034db31";
  nixpkgs = fetchTarball "https://github.com/NixOS/nixpkgs/archive/${rev}.tar.gz";
  pkgs = import nixpkgs {};
in with pkgs; [
    cmake
    pkg-config
    mesa
    mesa_glu
    clang 
    qt514.qtbase
]

with import <nixpkgs> {};
mkShell {
  buildInputs = [ 
    gcc 
    cmake 
    clang 
    clang-tools
    qt5.full
    qtcreator
];

}

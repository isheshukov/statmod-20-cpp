#include "mainwindow.h"
#include <QApplication>
#include <QtWidgets>
#include <iostream>
#include <run.hpp>
#include <structopt/app.hpp>

int
main(int argc, char* argv[])
{
  try {
    auto options =
      structopt::app("optimization_methods").parse<Options>(argc, argv);

    if (options.function == Options::FunctionName::easom &&
        options.initial_point.size() > 2) {
      std::cerr << "Error: Easom is a 2D function" << std::endl;
      exit(1);
    }

    if (!options.gui.value()) {
      std::cout << run::run(options) << std::endl;
      return 0;
    } else {
      QApplication a(argc, argv);
      MainWindow window;
      window.show();
      window.setWindowTitle(QApplication::translate(
        "toplevel", "C++ Multidimensional optimization"));
      return a.exec();
    }
  } catch (structopt::exception& e) {
    std::cout << e.what() << "\n";
    std::cout << e.help();
  }
  return 0;
}

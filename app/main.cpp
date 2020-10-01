#include "argh.h"
#include "mainwindow.h"
#include "optimization.hpp"
#include <Eigen/Dense>
#include <QApplication>
#include <QtWidgets>
#include <cmath>
#include <functional>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <unsupported/Eigen/MatrixFunctions>

using namespace Eigen;

int
main(int argc, char* argv[])
{
  std::map<std::string, std::function<double(VectorXd)>> functions = {
    { "sphere", [](VectorXd x) { return std::pow(x.norm(), 2); } },
    { "easom",
      [](VectorXd x) {
        auto xx = x.array() - M_PI;
        return (-cos(x[0]) * cos(x[1]) * exp(-xx[0] * xx[0] - xx[1] * xx[1]));
      } },
    { "rosenbrock",
      [](VectorXd x) {
        return pow(1 - x[0], 2) + 100 * pow(x[1] - x[0] * x[0], 2);
      } },
    { "rastrigin",
      [](VectorXd x) {
        auto x2 = x.array().pow(2) - 10 * cos(2 * M_PI * x.array());
        return 10 * x.size() + x2.sum();
      } }
  };

  argh::parser cmdl(argv, argh::parser::PREFER_PARAM_FOR_UNREG_OPTION);
  std::string functionName = cmdl({ "-f", "--function" }, "sphere").str();
  VectorXd initial_point = Vector2d({ 0, 0 });

  if (!cmdl[{ "--gui" }]) {
    auto stop_criterion =
      std::make_unique<Optimization::StopCriterion::MinStdDeviation>(1e-6);
    auto parameters = std::make_shared<Optimization::OptimizationParameters>();

    if (cmdl.pos_args().size() > 0) {
      initial_point.resize(cmdl.pos_args().size() - 1);
      for (size_t i = 1; i < cmdl.pos_args().size(); ++i)
        initial_point[i - 1] = std::stod(cmdl[i]);
    }

    parameters->initial_point = initial_point;

    parameters->function = functions.at(functionName);

    auto method =
      std::make_unique<Optimization::Method::NelderMead>(parameters);

    Optimization::optimize(std::move(method), std::move(stop_criterion));
    std::cout << *parameters << std::endl;
    return 0;
  } else {
    QApplication a(argc, argv);
    MainWindow window;
    window.show();
    window.setWindowTitle(
      QApplication::translate("toplevel", "C++ Multidimensional optimization"));
    return a.exec();
  }
}

#ifdef GUI
#include <QCoreApplication>
#endif
#include "optimization.hpp"
#include <Eigen/Dense>
#include <cmath>
#include <functional>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>

using namespace Eigen;

int
main(int argc, char* argv[])
{
#ifdef GUI
  QCoreApplication a(argc, argv);
#endif
  std::map<std::string, std::function<double(VectorXd)>> functions = {
    { "x^2", [](VectorXd x) { return x.norm(); } },
    { "x^3 + 5", [](VectorXd x) { return 4; } },
    { "abs(log(x))", [](VectorXd x) { return x.norm(); } }
  };

  {
    auto stop_criterion =
      std::make_unique<Optimization::StopCriterion::MaxIterations>(20);
    auto parameters = std::make_shared<Optimization::OptimizationParameters>();
    parameters->initial_point = Vector2d({ 2, 0.1 });
    parameters->function = functions["x^2"];

    auto method =
      std::make_unique<Optimization::Method::NelderMead>(parameters);

    Optimization::optimize(std::move(method), std::move(stop_criterion));
    std::cout << *parameters << std::endl;
  }

#ifdef GUI
  return a.exec();
#else
  return 0;
#endif
}

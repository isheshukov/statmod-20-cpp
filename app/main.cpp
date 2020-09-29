#ifdef GUI
#include <QCoreApplication>
#endif
#include "optimization.hpp"
#include <iostream>
#include <map>
#include <functional>
#include <iterator>
#include <cmath>
#include <Eigen/Dense>
#include <memory>

using namespace Eigen;

int main(int argc, char *argv[])
{
#ifdef GUI
    QCoreApplication a(argc, argv);
#endif
    std::map<std::string, std::function<double(VectorXd)>> functions = {
        {"x^2", [](VectorXd x){ return x.norm(); }},
        {"x^3 + 5", [](VectorXd x){ return 4; }},
        {"abs(log(x))", [](VectorXd x){ return x.norm(); }}
    };

    Optimization::StopCriterion::MaxIterations(10);
    Optimization::OptimizationParameters parameters;
    Optimization::Method::NelderMead method(&parameters);
    parameters.method = &method;
    parameters.stop_criterion = new Optimization::StopCriterion::MaxIterations(10);
    parameters.initial_point = Vector3d({0.1, 9, 1});
    parameters.function = functions["abs(log(x))"];

    Optimization::optimize(parameters);

    std::cout << "Hello World" << std::endl;


#ifdef GUI
    return a.exec();
#else
    return 0;
#endif
}

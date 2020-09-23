#ifdef GUI
#include <QCoreApplication>
#endif
#include <iostream>
#include "optimization.hpp"

int main(int argc, char *argv[])
{
#ifdef GUI
    QCoreApplication a(argc, argv);
#endif

    Optimization::StopCriterion::MaxIterations criterion(1000);
    Optimization::OptimizationParameters parameters;
    parameters.iteration_num = 1001;
    std::cout << "Hello World" << criterion.check(parameters) << std::endl;
#ifdef GUI
    return a.exec();
#else
    return 0;
#endif
}

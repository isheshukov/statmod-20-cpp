#pragma once

#include <iostream>
#include <vector>
#include <functional>
#include <Eigen/Dense>
#include "stop_criterion.hpp"
#include "method.hpp"

using namespace Eigen;

namespace Optimization {
struct OptimizationParameters {
    std::function<double(VectorXd)> function;
    VectorXd initial_point;
    Optimization::Method::AbstractMethod* method;
    Optimization::StopCriterion::AbstractCriterion* stop_criterion;
    size_t arg_dim = 1;
    size_t iteration_num = 0;
    size_t iteration_last_improvement_num = 0;
    double eps = 1e-6;
};
}


std::ostream& operator<<(std::ostream& os, const Optimization::OptimizationParameters& p) {
    os << "num=" << p.iteration_num
       << ", last improv num=" << p.iteration_last_improvement_num << ", current_point=(";

    //for (size_t i = 0; i < p.point_history.back().size(); ++i ) {
    //    os << p.point_history.back()[i] << ", ";
    //}
    return os << ")" << std::endl;
}

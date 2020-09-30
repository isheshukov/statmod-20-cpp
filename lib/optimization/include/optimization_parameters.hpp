#pragma once

#include <Eigen/Dense>
#include <functional>
#include <iostream>
#include <vector>

using namespace Eigen;

namespace Optimization {
struct OptimizationParameters
{
  std::function<double(VectorXd)> function;
  VectorXd initial_point;
  size_t arg_dim = 1;
  size_t iteration_num = 0;
  size_t iteration_last_improvement_num = 0;
  double eps = 1e-6;

  std::vector<std::pair<Eigen::VectorXd, double>> simplex;
  double initial_simplex_step = 1.0;
};
}

std::ostream&
operator<<(std::ostream& os, const Optimization::OptimizationParameters& p);

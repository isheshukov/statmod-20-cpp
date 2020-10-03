#pragma once

#include <Eigen/Dense>
#include <functional>
#include <iostream>
#include <vector>

using namespace Eigen;

namespace Optimization {
struct OptimizationParameters
{
  virtual bool f() { return true; };
  std::function<double(VectorXd)> function;
  VectorXd initial_point;
  size_t arg_dim = 1;
  size_t iteration_num = 0;
  size_t iteration_last_improvement_num = 0;
  double eps = 1e-6;
};

struct NelderMeadOptimizationParameters : public OptimizationParameters
{
  std::vector<std::pair<Eigen::VectorXd, double>> simplex;
  double initial_simplex_step = 1.0;
  virtual bool f() override { return true; }
};
}

std::ostream&
operator<<(std::ostream& os, const Optimization::OptimizationParameters& p);

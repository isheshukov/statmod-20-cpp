#pragma once

#include "mymath.hpp"
#include <Eigen/Dense>
#include <functional>
#include <iostream>
#include <memory>
#include <vector>

using namespace Eigen;

using namespace MyMath;
namespace Optimization {
struct OptimizationParameters
{
  virtual bool f() { return true; };
  std::function<double(VectorXd)> function;
  Box search_space;
  VectorXd initial_point;
  // size_t arg_dim = 1;
  double eps = 1e-6;
  PointVal current_best;
};

struct NelderMeadOptimizationParameters : public OptimizationParameters
{
  virtual bool f() override { return true; }
  std::vector<std::pair<Eigen::VectorXd, double>> simplex;
  double initial_simplex_step = 1.0;
};

struct RandomSearchOptimizationParameters : public OptimizationParameters
{
  virtual bool f() override { return true; }
  double p = 0.5;
  double delta = 1;
  double alpha = 0.5;
};

struct OptimizationState
{
  size_t iteration_num = 0;
  size_t iteration_no_improv = std::numeric_limits<size_t>::max();
  std::vector<PointVal> point_history;
  std::shared_ptr<OptimizationParameters> method_parameters;
};

}

std::ostream&
operator<<(std::ostream& os, const Optimization::OptimizationParameters& p);

std::ostream&
operator<<(std::ostream& os, const Optimization::OptimizationState& p);

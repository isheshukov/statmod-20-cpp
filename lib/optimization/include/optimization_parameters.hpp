#pragma once

#include "mymath.hpp"
#include <Eigen/Dense>
#include <functional>
#include <iostream>
#include <memory>
#include <vector>

namespace Optimization {
struct OptimizationParameters
{
  virtual ~OptimizationParameters() = default;
  std::function<double(Eigen::VectorXd)> function;
  MyMath::Box search_space;
  Eigen::VectorXd initial_point;
  // size_t arg_dim = 1;
  double eps = 1e-6;
  MyMath::PointVal current_best;
};

struct NelderMeadOptimizationParameters : public OptimizationParameters
{
  virtual ~NelderMeadOptimizationParameters() = default;
  std::vector<std::pair<Eigen::VectorXd, double>> simplex;
  double initial_simplex_step = 1.0;
};

struct RandomSearchOptimizationParameters : public OptimizationParameters
{
  virtual ~RandomSearchOptimizationParameters() = default;
  double p = 0.5;
  double delta = 1;
  double alpha = 0.5;
};

struct OptimizationState
{
  size_t iteration_num = 0;
  size_t iteration_no_improv = std::numeric_limits<size_t>::max();
  std::vector<MyMath::PointVal> point_history;
  std::shared_ptr<OptimizationParameters> method_parameters;
};

}

std::ostream&
operator<<(std::ostream& os, const Optimization::OptimizationParameters& p);

std::ostream&
operator<<(std::ostream& os, const Optimization::OptimizationState& p);

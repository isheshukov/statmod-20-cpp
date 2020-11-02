#pragma once

#include "mymath.hpp"
#include <Eigen/Dense>
#include <functional>
#include <iostream>
#include <memory>
#include <variant>
#include <vector>

namespace Optimization {
namespace Parameters {

struct NelderMead
{
  std::function<double(Eigen::VectorXd)> function;
  MyMath::Box search_space;
  Eigen::VectorXd initial_point;
  double eps = 1e-6;
  MyMath::PointVal current_best;
  std::vector<std::pair<Eigen::VectorXd, double>> simplex;
  double initial_simplex_step = 1.0;
};

struct RandomSearch
{
  std::function<double(Eigen::VectorXd)> function;
  MyMath::Box search_space;
  Eigen::VectorXd initial_point;
  double eps = 1e-6;
  MyMath::PointVal current_best;
  double p = 0.5;
  double delta = 1;
  double alpha = 0.5;
};

using ParametersVariant = std::variant<NelderMead, RandomSearch>;
}

struct OptimizationState
{
  size_t iteration_num = 0;
  size_t iteration_no_improv = std::numeric_limits<size_t>::max();
  std::vector<MyMath::PointVal> point_history;
  Parameters::ParametersVariant method_parameters;
};

}

std::ostream&
operator<<(std::ostream& os,
           const Optimization::Parameters::ParametersVariant& p);

std::ostream&
operator<<(std::ostream& os, const Optimization::OptimizationState& p);

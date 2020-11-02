#pragma once

#include <Eigen/Dense>
#include <algorithm>
#include <iostream>
#include <memory>
#include <mymath.hpp>
#include <numeric>
#include <optimization_parameters.hpp>
#include <variant>

/**
 * @brief PairSecondCmp comparator
 * Compares pairs by second field.
 */

template <class U, class V>
struct PairSecondCmp {
  bool operator()(const std::pair<U, V>& a, const std::pair<U, V>& b) {
    return a.second < b.second;
  }
};

namespace Optimization {
namespace Method {

class NelderMead {
 public:
  NelderMead() = default;
  NelderMead(std::function<double(Eigen::VectorXd)> _function,
             MyMath::Box _search_space,
             Eigen::VectorXd _initial_point,
             double _initial_simplex_step);
  MyMath::PointVal next();

  std::function<double(Eigen::VectorXd)> function;
  MyMath::Box search_space;
  Eigen::VectorXd initial_point;
  MyMath::PointVal current_best;
  std::vector<std::pair<Eigen::VectorXd, double>> simplex;
  double initial_simplex_step = 1.0;
};

class RandomSearch {
 public:
  RandomSearch() = default;
  RandomSearch(std::function<double(Eigen::VectorXd)> _function,
               MyMath::Box _search_space,
               Eigen::VectorXd _initial_point,
               double _p,
               double _delta,
               double _alpha);

  MyMath::PointVal next();

  std::function<double(Eigen::VectorXd)> function;
  MyMath::Box search_space;
  Eigen::VectorXd initial_point;
  MyMath::PointVal current_best;
  double p = 0.5;
  double delta = 1;
  double alpha = 0.5;
};

using MethodVariant = std::variant<NelderMead, RandomSearch>;

}  // namespace Method
}  // namespace Optimization

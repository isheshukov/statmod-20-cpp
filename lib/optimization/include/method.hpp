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

template<class U, class V>
struct PairSecondCmp
{
  bool operator()(const std::pair<U, V>& a, const std::pair<U, V>& b)
  {
    return a.second < b.second;
  }
};

namespace Optimization {
namespace Method {

class NelderMead
{
public:
  NelderMead() = default;
  MyMath::PointVal next();
  void setParameters(const Optimization::Parameters::NelderMead& p);
  Optimization::Parameters::NelderMead& getParameters() { return parameters; }
  Optimization::Parameters::NelderMead parameters;
};

class RandomSearch
{
public:
  RandomSearch() = default;
  MyMath::PointVal next();
  void setParameters(const Optimization::Parameters::RandomSearch& p);
  Optimization::Parameters::RandomSearch& getParameters() { return parameters; }
  Optimization::Parameters::RandomSearch parameters;
};

using MethodVariant = std::variant<NelderMead, RandomSearch>;

}
}

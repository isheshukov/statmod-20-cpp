#pragma once

#include <Eigen/Dense>
#include <algorithm>
#include <iostream>
#include <memory>
#include <numeric>
#include <optimization_parameters.hpp>

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
class AbstractMethod
{
public:
  AbstractMethod(std::shared_ptr<OptimizationParameters> p)
  {
    this->parameters = p;
  }
  virtual void next() {}
  std::shared_ptr<OptimizationParameters> getParameters() { return parameters; }

  std::shared_ptr<OptimizationParameters> parameters;
};

class NelderMead : public AbstractMethod
{
  std::vector<std::pair<Eigen::VectorXd, double>> simplex;
  double initial_simplex_step = 1.0;

public:
  NelderMead(std::shared_ptr<OptimizationParameters> p);
  virtual void next() override;
};

}
}

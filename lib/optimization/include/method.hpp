#pragma once

#include <Eigen/Dense>
#include <algorithm>
#include <iostream>
#include <memory>
#include <numeric>
#include <optimization_parameters.hpp>

using PointVal = std::pair<Eigen::VectorXd, double>;
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
  virtual ~AbstractMethod(){};
  virtual double next() = 0;
  std::shared_ptr<OptimizationParameters> getParameters() { return parameters; }
  std::shared_ptr<OptimizationParameters> parameters;
};

class NelderMead : public AbstractMethod
{
public:
  NelderMead(std::shared_ptr<OptimizationParameters> p);
  virtual double next() override;

private:
  PointVal createPoint(Eigen::VectorXd v, std::function<double(VectorXd)>& f);
};

class RandomSearch : public AbstractMethod
{
public:
  RandomSearch(std::shared_ptr<OptimizationParameters> p);
  virtual double next() override;
};

}
}

#pragma once

#include "optimization_parameters.hpp"
#include <memory>

namespace Optimization {
namespace StopCriterion {

struct AbstractCriterion
{
  virtual bool check(std::shared_ptr<OptimizationParameters> p) = 0;
  virtual ~AbstractCriterion() {}
};

struct PointDistance : public AbstractCriterion
{
  bool check(std::shared_ptr<OptimizationParameters> p);
};

struct MaxIterations : public AbstractCriterion
{
  MaxIterations(unsigned int N);
  bool check(std::shared_ptr<OptimizationParameters> p);

private:
  unsigned int m_N;
};

struct MaxIterationsSinceImprovement : public AbstractCriterion
{
  MaxIterationsSinceImprovement(unsigned int N);
  bool check(std::shared_ptr<OptimizationParameters> p);

private:
  unsigned int m_N;
};

struct MinStdDeviation : public AbstractCriterion
{
  MinStdDeviation(double eps);
  bool check(std::shared_ptr<OptimizationParameters> p);

private:
  double m_eps;
};

}
}

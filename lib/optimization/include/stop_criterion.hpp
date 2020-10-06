#pragma once

#include "optimization_parameters.hpp"
#include <memory>

namespace Optimization {
namespace StopCriterion {

struct AbstractCriterion
{
  virtual bool check(const OptimizationState& p) const = 0;
  virtual ~AbstractCriterion() {}
};

struct PointDistance : public AbstractCriterion
{
  bool check(const OptimizationState& p) const;
};

struct MaxIterations : public AbstractCriterion
{
  MaxIterations(unsigned int N);
  bool check(const OptimizationState& p) const;

private:
  unsigned int m_N;
};

struct MaxIterationsSinceImprovement : public AbstractCriterion
{
  MaxIterationsSinceImprovement(unsigned int N);
  bool check(const OptimizationState& p) const;

private:
  unsigned int m_N;
};

struct MinStdDeviation : public AbstractCriterion
{
  MinStdDeviation(double eps);
  bool check(const OptimizationState& p) const;

private:
  double m_eps;
};

}
}

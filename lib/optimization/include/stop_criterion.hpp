#pragma once

#include "optimization_parameters.hpp"
#include <memory>
#include <variant>

namespace Optimization {
namespace StopCriterion {

struct PointDistance
{
  bool check(const Optimization::OptimizationState& p) const;
};

struct MaxIterations
{
  MaxIterations(unsigned int N);
  bool check(const Optimization::OptimizationState& p) const;

private:
  unsigned int m_N;
};

struct MaxIterationsSinceImprovement
{
  MaxIterationsSinceImprovement(unsigned int N);
  bool check(const Optimization::OptimizationState& p) const;

private:
  unsigned int m_N;
};

struct MinStdDeviation
{
  MinStdDeviation(double eps, unsigned int N);
  bool check(const Optimization::OptimizationState& p) const;

private:
  double m_eps;
  unsigned int m_N;
};

using StopCriterionVariant = std::variant<PointDistance,
                                          MaxIterations,
                                          MaxIterationsSinceImprovement,
                                          MinStdDeviation>;

}
}

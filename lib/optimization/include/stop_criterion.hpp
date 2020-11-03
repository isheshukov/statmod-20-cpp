#pragma once

#include <memory>
#include <variant>
#include "method.hpp"
#include "optimization_parameters.hpp"

namespace Optimization {
namespace StopCriterion {

struct PointDistance {
  bool check(const Optimization::State& p,
             const Optimization::Method::MethodVariant& m) const;
};

struct MaxIterations {
  MaxIterations(unsigned int N);
  bool check(const Optimization::State& p,
             const Optimization::Method::MethodVariant& m) const;

 private:
  unsigned int m_N;
};

struct MaxIterationsSinceImprovement {
  MaxIterationsSinceImprovement(unsigned int N);
  bool check(const Optimization::State& p,
             const Optimization::Method::MethodVariant& m) const;

 private:
  unsigned int m_N;
};

struct MinStdDeviation {
  MinStdDeviation(double eps, unsigned int N);
  bool check(const Optimization::State& p,
             const Optimization::Method::MethodVariant& m) const;

 private:
  double m_eps;
  unsigned int m_N;
};

using StopCriterionVariant = std::variant<PointDistance,
                                          MaxIterations,
                                          MaxIterationsSinceImprovement,
                                          MinStdDeviation>;

}  // namespace StopCriterion
}  // namespace Optimization

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

/**
 * @brief Max iterations stop critertion
 * @details Stops when number of iterations is over the threshold
 */
struct MaxIterations {
  MaxIterations(unsigned int N);
  bool check(const Optimization::State& p,
             const Optimization::Method::MethodVariant& m) const;

 private:
  unsigned int m_N;  ///< Threshold
};

/**
 * @brief Max iterations since improvement stop critertion
 * @details Stops when number of iterations since last improvement is over the
 * threshold
 */
struct MaxIterationsSinceImprovement {
  MaxIterationsSinceImprovement(unsigned int N);
  bool check(const Optimization::State& p,
             const Optimization::Method::MethodVariant& m) const;

 private:
  unsigned int m_N;  ///< Threshold
};

/**
 * @brief Minimal standard deviation stop critertion
 * @details Stops when standard deviation of function values at points of a
 * simplex is less than a threshold OR If number of iterations is over the
 * threshold
 */
struct MinStdDeviation {
  MinStdDeviation(double eps, unsigned int N);
  bool check(const Optimization::State& p,
             const Optimization::Method::MethodVariant& m) const;

 private:
  double m_eps;      // Threshold
  unsigned int m_N;  // Max iterations threshold
};

using StopCriterionVariant = std::variant<PointDistance,
                                          MaxIterations,
                                          MaxIterationsSinceImprovement,
                                          MinStdDeviation>;

}  // namespace StopCriterion
}  // namespace Optimization

#include <Eigen/Dense>
#include <stop_criterion.hpp>

using namespace Eigen;

bool
Optimization::StopCriterion::PointDistance::check(
  const OptimizationState& p) const
{
  return false;
}

Optimization::StopCriterion::MaxIterations::MaxIterations(unsigned int N)
  : m_N(N)
{}
bool
Optimization::StopCriterion::MaxIterations::check(
  const OptimizationState& p) const
{
  return p.iteration_num < m_N;
}

Optimization::StopCriterion::MaxIterationsSinceImprovement::
  MaxIterationsSinceImprovement(unsigned int N)
  : m_N(N)
{}
bool
Optimization::StopCriterion::MaxIterationsSinceImprovement::check(
  const OptimizationState& p) const
{
  return (p.iteration_num - p.iteration_no_improv) < m_N;
}

Optimization::StopCriterion::MinStdDeviation::MinStdDeviation(double eps)
  : m_eps(eps)
{}
bool
Optimization::StopCriterion::MinStdDeviation::check(
  const OptimizationState& p) const
{
  VectorXd values;
  auto pp = std::dynamic_pointer_cast<NelderMeadOptimizationParameters>(
    p.method_parameters);
  values.resize(pp->simplex.size());
  for (size_t i = 0; i < pp->simplex.size(); ++i) {
    values[i] = pp->simplex[i].second;
  }

  return std::sqrt((values.array() - values.mean()).square().sum() /
                   (values.size() - 1)) > m_eps;
}

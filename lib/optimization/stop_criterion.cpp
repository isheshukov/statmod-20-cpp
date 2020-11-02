#include <Eigen/Dense>
#include <stop_criterion.hpp>

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

Optimization::StopCriterion::MinStdDeviation::MinStdDeviation(double eps,
                                                              unsigned int N)
  : m_eps(eps)
  , m_N(N)
{}
bool
Optimization::StopCriterion::MinStdDeviation::check(
  const OptimizationState& p) const
{
  Eigen::VectorXd values;
  if (auto x = std::get_if<Optimization::Parameters::NelderMead>(
        &(p.method_parameters))) {
    values.resize(x->simplex.size());
    for (size_t i = 0; i < x->simplex.size(); ++i) {
      values[i] = x->simplex[i].second;
    }
  }
  return (std::sqrt((values.array() - values.mean()).square().sum() /
                    (values.size() - 1)) > m_eps) &&
         (p.iteration_num < m_N);
}

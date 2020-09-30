#include <Eigen/Dense>
#include <stop_criterion.hpp>

using namespace Eigen;

bool
Optimization::StopCriterion::PointDistance::check(
  std::shared_ptr<OptimizationParameters> p)
{
  return false;
}

Optimization::StopCriterion::MaxIterations::MaxIterations(unsigned int N)
  : m_N(N)
{}
bool
Optimization::StopCriterion::MaxIterations::check(
  std::shared_ptr<OptimizationParameters> p)
{
  return p->iteration_num < m_N;
}

Optimization::StopCriterion::MaxIterationsSinceImprovement::
  MaxIterationsSinceImprovement(unsigned int N)
  : m_N(N)
{}
bool
Optimization::StopCriterion::MaxIterationsSinceImprovement::check(
  std::shared_ptr<OptimizationParameters> p)
{
  return (p->iteration_num - p->iteration_last_improvement_num) < m_N;
}

Optimization::StopCriterion::MinStdDeviation::MinStdDeviation(double eps)
  : m_eps(eps)
{}
bool
Optimization::StopCriterion::MinStdDeviation::check(
  std::shared_ptr<OptimizationParameters> p)
{
  VectorXd values;
  values.resize(p->simplex.size());
  for (size_t i = 0; i < p->simplex.size(); ++i) {
    values[i] = p->simplex[i].second;
  }

  return std::sqrt((values.array() - values.mean()).square().sum() /
                   (values.size() - 1)) > m_eps;
}

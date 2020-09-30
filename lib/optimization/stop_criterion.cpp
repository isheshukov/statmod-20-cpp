#include <stop_criterion.hpp>

bool
Optimization::StopCriterion::AbstractCriterion::check(
  std::shared_ptr<OptimizationParameters> p)
{
  return false;
}

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

#include <Eigen/Dense>
#include <stop_criterion.hpp>

bool Optimization::StopCriterion::PointDistance::check(
    const State& p,
    const Optimization::Method::MethodVariant& m) const {
  return false;
}

Optimization::StopCriterion::MaxIterations::MaxIterations(unsigned int N)
    : m_N(N) {}
bool Optimization::StopCriterion::MaxIterations::check(
    const State& p,
    const Optimization::Method::MethodVariant& m) const {
  return p.iteration_num < m_N;
}

Optimization::StopCriterion::MaxIterationsSinceImprovement::
    MaxIterationsSinceImprovement(unsigned int N)
    : m_N(N) {}
bool Optimization::StopCriterion::MaxIterationsSinceImprovement::check(
    const State& p,
    const Optimization::Method::MethodVariant& m) const {
  return (p.iteration_num - p.iteration_no_improv) < m_N;
}

Optimization::StopCriterion::MinStdDeviation::MinStdDeviation(double eps,
                                                              unsigned int N)
    : m_eps(eps), m_N(N) {}
bool Optimization::StopCriterion::MinStdDeviation::check(
    const State& p,
    const Optimization::Method::MethodVariant& m) const {
  Eigen::VectorXd values;
  if (auto x = std::get_if<Optimization::Method::NelderMead>(&(m))) {
    values.resize(x->simplex.size());
    for (size_t i = 0; i < x->simplex.size(); ++i) {
      values[i] = x->simplex[i].second;
    }
  }

  auto mean = values.sum() / values.rows();
  auto std = ((values.array() - mean).square().sum() / values.rows());

  return (std > this->m_eps) && (p.iteration_num < this->m_N);
}

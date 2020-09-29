#include <stop_criterion.hpp>

bool Optimization::StopCriterion::PointDistance::check(OptimizationParameters p) {
    return false;
}

Optimization::StopCriterion::MaxIterations::MaxIterations(unsigned int N) : m_N(N) {}
bool Optimization::StopCriterion::MaxIterations::check(OptimizationParameters p) {
    return p.iteration_num < m_N;
}

Optimization::StopCriterion::MaxIterationsSinceImprovement::MaxIterationsSinceImprovement(unsigned int N) : m_N(N) {}
bool Optimization::StopCriterion::MaxIterationsSinceImprovement::check(OptimizationParameters p) {
    return (p.iteration_num - p.iteration_last_improvement_num) < m_N;
}

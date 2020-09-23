#include "optimization_parameters.hpp"

namespace StopCriterion {

class AbstractCriterion {
public:
   virtual bool check(OptimizationParameters p) = 0;
private:
};

class PointDistance : AbstractCriterion{
public:
    bool check(OptimizationParameters p) {
        return false;
    }
};

class MaxIterations : AbstractCriterion{
public:
    MaxIterations(unsigned int N) : m_N(N) {}
    bool check(OptimizationParameters p) {
        return p.iteration_num < m_N;
    }
private:
    unsigned int m_N;
};

class MaxIterationsSinceImprovement : AbstractCriterion{
public:
    MaxIterationsSinceImprovement(unsigned int N) : m_N(N) {}
    bool check(OptimizationParameters p) {
        return (p.iteration_num - p.iteration_last_improvement_num) < m_N;
    }
private:
    unsigned int m_N;
};

}

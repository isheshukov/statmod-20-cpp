#pragma once

#include "optimization_parameters.hpp"

namespace Optimization {
namespace StopCriterion {
class OptimizationParameters;

struct AbstractCriterion {
    virtual bool check(OptimizationParameters p);
};

struct PointDistance : public AbstractCriterion{
    virtual bool check(OptimizationParameters p) override;
};

struct MaxIterations : public AbstractCriterion{
    MaxIterations(unsigned int N);
    virtual bool check(OptimizationParameters p) override;
private:
    unsigned int m_N;
};

struct MaxIterationsSinceImprovement : public AbstractCriterion{
    MaxIterationsSinceImprovement(unsigned int N);
    virtual bool check(OptimizationParameters p) override;
private:
    unsigned int m_N;
};

}
}

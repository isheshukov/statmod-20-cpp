#pragma once

#include "optimization_parameters.hpp"
#include <memory>

namespace Optimization {
namespace StopCriterion {

struct AbstractCriterion
{
  virtual bool check(std::shared_ptr<OptimizationParameters> p);
};

struct PointDistance : public AbstractCriterion
{
  virtual bool check(std::shared_ptr<OptimizationParameters> p) override;
};

struct MaxIterations : public AbstractCriterion
{
  MaxIterations(unsigned int N);
  virtual bool check(std::shared_ptr<OptimizationParameters> p) override;

private:
  unsigned int m_N;
};

struct MaxIterationsSinceImprovement : public AbstractCriterion
{
  MaxIterationsSinceImprovement(unsigned int N);
  virtual bool check(std::shared_ptr<OptimizationParameters> p) override;

private:
  unsigned int m_N;
};

}
}

#include "method.hpp"
#include "optimization_parameters.hpp"
#include "stop_criterion.hpp"
#include <memory>

namespace Optimization {
PointVal
optimize(
  std::unique_ptr<Optimization::Method::AbstractMethod> method,
  std::unique_ptr<Optimization::StopCriterion::AbstractCriterion> criterion);
}

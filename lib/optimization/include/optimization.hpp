#include "method.hpp"
#include "optimization_parameters.hpp"
#include "stop_criterion.hpp"
#include <memory>

namespace Optimization {
OptimizationState
optimize(Optimization::Method::MethodVariant& method,
         Optimization::StopCriterion::StopCriterionVariant& criterion);
}

#include <memory>
#include "method.hpp"
#include "optimization_parameters.hpp"
#include "stop_criterion.hpp"

namespace Optimization {
State optimize(Optimization::Method::MethodVariant& method,
               Optimization::StopCriterion::StopCriterionVariant& criterion);
}

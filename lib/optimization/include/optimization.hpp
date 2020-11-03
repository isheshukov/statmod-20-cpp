#include <memory>
#include "method.hpp"
#include "optimization_parameters.hpp"
#include "stop_criterion.hpp"

namespace Optimization {
/**
 * @brief Optimize function
 * @returns State
 */
State optimize(Optimization::Method::MethodVariant& method,
               Optimization::StopCriterion::StopCriterionVariant& criterion);
}  // namespace Optimization

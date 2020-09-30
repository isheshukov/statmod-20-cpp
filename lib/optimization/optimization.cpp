#include <memory>
#include <method.hpp>
#include <stop_criterion.hpp>

namespace Optimization {
void
optimize(
  std::unique_ptr<Optimization::Method::AbstractMethod> method,
  std::unique_ptr<Optimization::StopCriterion::AbstractCriterion> criterion)
{
  do {
    method->next();
  } while (criterion->check(method->getParameters()));
}
}

#include <memory>
#include <method.hpp>
#include <stop_criterion.hpp>

namespace Optimization {
PointVal
optimize(
  std::unique_ptr<Optimization::Method::AbstractMethod> method,
  std::unique_ptr<Optimization::StopCriterion::AbstractCriterion> criterion)
{
  OptimizationState state;
  state.method_parameters = method->getParameters();
  do {
    state.point_history.push_back(method->next());
    state.iteration_num++;
    std::cout << state.point_history.back() << std::endl;
  } while (criterion->check(state));

  return state.point_history.back();
}
}

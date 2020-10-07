#include <memory>
#include <method.hpp>
#include <stop_criterion.hpp>

namespace Optimization {
OptimizationState
optimize(
  std::unique_ptr<Optimization::Method::AbstractMethod> method,
  std::unique_ptr<Optimization::StopCriterion::AbstractCriterion> criterion)
{
  OptimizationState state;
  state.method_parameters = method->getParameters();
  state.point_history.push_back(state.method_parameters->current_best);
  state.point_history.push_back(state.method_parameters->current_best);
  state.point_history.push_back(state.method_parameters->current_best);
  state.point_history.push_back(state.method_parameters->current_best);
  do {
    state.point_history.push_back(method->next());
    state.iteration_num++;
    state.iteration_no_improv++;

    if (state.point_history.back().second <
        (state.point_history.end() - 2)->second)
      state.iteration_no_improv = 0;
  } while (criterion->check(state));

  return state;
}
}

#include <memory>
#include <method.hpp>
#include <stop_criterion.hpp>

namespace Optimization {
State optimize(Optimization::Method::MethodVariant& method,
               Optimization::StopCriterion::StopCriterionVariant& criterion) {
  State state;

  auto current_best =
      std::visit([](auto& x) { return x.current_best; }, method);
  auto getNext = [](auto& x) { return x.next(); };
  auto check = [&state, &method](auto& criterion) {
    return criterion.check(state, method);
  };
  state.point_history.push_back(current_best);

  do {
    state.point_history.push_back(std::visit(getNext, method));
    state.iteration_num++;
    state.iteration_no_improv++;

    if (state.point_history.back().second <
        (state.point_history.end() - 2)->second)
      state.iteration_no_improv = 0;
  } while (std::visit(check, criterion));

  return state;
}
}  // namespace Optimization

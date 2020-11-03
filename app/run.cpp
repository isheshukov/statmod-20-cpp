#include "run.hpp"
#include <memory>
#include <optimization.hpp>
#include <utility>

Optimization::State run::run(Options options) {
  Optimization::StopCriterion::StopCriterionVariant stop_criterion;

  switch (options.stop_criterion.value()) {
    case Options::StopCriterion::min_std_dev:
      stop_criterion = Optimization::StopCriterion::MinStdDeviation(
          options.eps.value(), options.max_iterations.value());
      break;

    case Options::StopCriterion::num_iterations:
      stop_criterion = Optimization::StopCriterion::MaxIterations(
          options.max_iterations.value());
      break;
  }

  Optimization::Method::MethodVariant method;

  Eigen::VectorXd point = Eigen::VectorXd::Map(options.initial_point.data(),
                                               options.initial_point.size());

  std::vector<std::pair<double, double>> search_space = {
      std::pair<double, double>(options.xStart.value(), options.xEnd.value()),
      std::pair<double, double>(options.yStart.value(), options.yEnd.value())};

  auto& function = functions.at(options.function.value());
  auto space = (point.size() == 2) ? search_space : MyMath::Box(point, 10);

  switch (options.method.value()) {
    case Options::Method::nelder_mead: {
      method = Optimization::Method::NelderMead(
          function, space, point, options.initial_simplex_step.value());
    } break;

    case Options::Method::random: {
      method = Optimization::Method::RandomSearch(
          function, space, point, options.explore_probability.value(),
          options.delta.value(), options.alpha.value());
    } break;
  }

  return Optimization::optimize(method, stop_criterion);
}

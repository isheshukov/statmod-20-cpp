#include "run.hpp"
#include <memory>
#include <optimization.hpp>

Optimization::OptimizationState
run::run(Options options)
{
  std::unique_ptr<Optimization::StopCriterion::AbstractCriterion>
    stop_criterion;

  switch (options.stop_criterion.value()) {
    case Options::StopCriterion::min_std_dev:
      stop_criterion =
        std::make_unique<Optimization::StopCriterion::MinStdDeviation>(
          options.eps.value());
      break;
    case Options::StopCriterion::num_iterations:
      stop_criterion =
        std::make_unique<Optimization::StopCriterion::MaxIterations>(
          options.max_iterations.value());
      break;
  }

  std::shared_ptr<Optimization::OptimizationParameters> parameters;

  switch (options.method.value()) {
    case Options::Method::nelder_mead:
      parameters =
        std::make_shared<Optimization::NelderMeadOptimizationParameters>();
      break;
    case Options::Method::random:
      parameters =
        std::make_shared<Optimization::RandomSearchOptimizationParameters>();

      std::dynamic_pointer_cast<
        Optimization::RandomSearchOptimizationParameters>(parameters)
        ->p = options.explore_probability.value();
      break;
  }

  // VectorXd point(options.initial_point);
  VectorXd point =
    VectorXd::Map(options.initial_point.data(), options.initial_point.size());

  parameters->function = functions.at(options.function.value());

  parameters->initial_point = point;
  parameters->current_best = createPointVal(point, parameters->function);
  parameters->search_space = Box(point, 10);

  std::unique_ptr<Optimization::Method::AbstractMethod> method;

  switch (options.method.value()) {
    case Options::Method::nelder_mead:
      method = std::make_unique<Optimization::Method::NelderMead>(parameters);
      break;
    case Options::Method::random:
      method = std::make_unique<Optimization::Method::RandomSearch>(parameters);
      break;
  }

  return Optimization::optimize(std::move(method), std::move(stop_criterion));
}

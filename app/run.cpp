#include "run.hpp"
#include <memory>
#include <optimization.hpp>
#include <utility>

Optimization::OptimizationState
run::run(Options options)
{
  std::unique_ptr<Optimization::StopCriterion::AbstractCriterion>
    stop_criterion;

  switch (options.stop_criterion.value()) {

    case Options::StopCriterion::min_std_dev:
      stop_criterion =
        std::make_unique<Optimization::StopCriterion::MinStdDeviation>(
          options.eps.value(), options.max_iterations.value());
      break;

    case Options::StopCriterion::num_iterations:
      stop_criterion =
        std::make_unique<Optimization::StopCriterion::MaxIterations>(
          options.max_iterations.value());
      break;
  }

  std::shared_ptr<Optimization::OptimizationParameters> parameters;

  switch (options.method.value()) {

    case Options::Method::nelder_mead: {
      parameters =
        std::make_shared<Optimization::NelderMeadOptimizationParameters>();
      auto pn = std::dynamic_pointer_cast<
        Optimization::NelderMeadOptimizationParameters>(parameters);
      pn->initial_simplex_step = options.initial_simplex_step.value();
      break;
    }

    case Options::Method::random: {
      parameters =
        std::make_shared<Optimization::RandomSearchOptimizationParameters>();

      auto pr = std::dynamic_pointer_cast<
        Optimization::RandomSearchOptimizationParameters>(parameters);
      pr->p = options.explore_probability.value();
      pr->delta = options.delta.value();
      pr->alpha = options.alpha.value();
      break;
    }
  }

  VectorXd point =
    VectorXd::Map(options.initial_point.data(), options.initial_point.size());

  parameters->function = functions.at(options.function.value());
  parameters->initial_point = point;
  parameters->current_best = createPointVal(point, parameters->function);
  std::vector<std::pair<double, double>> search_space = {
    std::pair<double, double>(options.xStart.value(), options.xEnd.value()),
    std::pair<double, double>(options.yStart.value(), options.yEnd.value())
  };
  parameters->search_space = search_space;

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

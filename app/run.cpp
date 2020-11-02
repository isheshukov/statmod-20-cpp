#include "run.hpp"
#include <memory>
#include <optimization.hpp>
#include <utility>

Optimization::OptimizationState
run::run(Options options)
{
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

  Optimization::Parameters::ParametersVariant parameters;
  Optimization::Method::MethodVariant method;

  Eigen::VectorXd point = Eigen::VectorXd::Map(options.initial_point.data(),
                                               options.initial_point.size());

  std::vector<std::pair<double, double>> search_space = {
    std::pair<double, double>(options.xStart.value(), options.xEnd.value()),
    std::pair<double, double>(options.yStart.value(), options.yEnd.value())
  };

  /*
  std::visit(
    [&options, &point, &search_space](auto& x) {
      x.function = functions.at(options.function.value());
      x.initial_point = point;
      x.current_best = MyMath::createPointVal(point, x.function);
      x.search_space =
        (point.size() == 2) ? search_space : MyMath::Box(point, 10);
    },
    parameters);
    */

  switch (options.method.value()) {

    case Options::Method::nelder_mead: {
      parameters = Optimization::Parameters::NelderMead();
      method = Optimization::Method::NelderMead();
      if (auto p =
            std::get_if<Optimization::Parameters::NelderMead>(&parameters))
        if (auto m = std::get_if<Optimization::Method::NelderMead>(&method)) {
          p->initial_simplex_step = options.initial_simplex_step.value();

          p->function = functions.at(options.function.value());
          p->initial_point = point;
          p->current_best = MyMath::createPointVal(point, p->function);
          p->search_space =
            (point.size() == 2) ? search_space : MyMath::Box(point, 10);

          m->setParameters(*p);
        }

      break;
    }

    case Options::Method::random: {
      parameters = Optimization::Parameters::RandomSearch();
      method = Optimization::Method::RandomSearch();

      if (auto p =
            std::get_if<Optimization::Parameters::RandomSearch>(&parameters))
        if (auto m = std::get_if<Optimization::Method::RandomSearch>(&method)) {
          p->p = options.explore_probability.value();
          p->delta = options.delta.value();
          p->alpha = options.alpha.value();

          p->function = functions.at(options.function.value());
          p->initial_point = point;
          p->current_best = MyMath::createPointVal(point, p->function);
          p->search_space =
            (point.size() == 2) ? search_space : MyMath::Box(point, 10);

          m->setParameters(*p);
        }
      break;
    }
  }

  return Optimization::optimize(method, stop_criterion);
}

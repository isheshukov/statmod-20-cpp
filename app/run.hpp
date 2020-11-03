#pragma once

#include <Eigen/Dense>
#include <QtCore>
#include <functional>
#include <map>
#include <optimization_parameters.hpp>
#include <optional>
#include <structopt/app.hpp>
#include <vector>

/**
  @brief Options struct
*/
struct Options {
 public:
  /**
    @brief Available stop criteria
  */
  enum class StopCriterion { min_std_dev, num_iterations };
  std::optional<StopCriterion> stop_criterion = StopCriterion::num_iterations;

  std::optional<bool> gui = false;  ///< If set starts program in GUI mode
  std::optional<size_t> max_iterations =
      100;  ///< Maximum number of iterations.
  std::optional<double> eps =
      1e-6;  ///< Threshold for MinStdDeviation stop criterion

  /** @name Random search options
   */
  ///@{
  std::optional<double> explore_probability =
      0.7;  ///< Probability of exploration in random search
  std::optional<double> delta = 1;    ///< Initial ball radius
  std::optional<double> alpha = 0.5;  ///< Radius shrink coefficient
  ///@}

  std::optional<double> initial_simplex_step =
      1.0;  ///< Step which is used to create initial simplex

  /** @name Search space
   *  This variables define a box in 2D space.
   */
  ///@{
  std::optional<double> xStart = 0;
  std::optional<double> xEnd = 1.0;
  std::optional<double> yStart = 0;
  std::optional<double> yEnd = 1.0;
  ///@}

  /**
    @brief Available methods
  */
  enum class Method { nelder_mead, random };
  std::optional<Method> method = Method::nelder_mead;

  /**
    @brief Available functioncs
  */
  enum class FunctionName { sphere, easom, rosenbrock, rastrigin, himmelblau };
  std::optional<FunctionName> function = FunctionName::sphere;

  std::vector<double> initial_point = {0,
                                       0};  ///< Starting point of optimizations
};

/// \cond DO_NOT_DOCUMENT
STRUCTOPT(Options,
          stop_criterion,
          gui,
          max_iterations,
          explore_probability,
          eps,
          delta,
          alpha,
          initial_simplex_step,
          xStart,
          xEnd,
          yStart,
          yEnd,
          method,
          function,
          initial_point);
/// \end DO_NOT_DOCUMENT

/**
  @brief Test functions for optimization
*/
static std::map<Options::FunctionName,
                std::function<double(Eigen::VectorXd)>,
                std::less<>>
    functions = {{Options::FunctionName::sphere,
                  [](Eigen::VectorXd x) { return std::pow(x.norm(), 2); }},
                 {Options::FunctionName::easom,
                  [](Eigen::VectorXd x) {
                    auto xx = x.array() - M_PI;
                    return (-cos(x[0]) * cos(x[1]) *
                            exp(-xx[0] * xx[0] - xx[1] * xx[1]));
                  }},
                 {Options::FunctionName::rosenbrock,
                  [](Eigen::VectorXd _x) {
                    auto n = _x.size();
                    auto x = _x.array().tail(n - 1);
                    auto y = _x.array().head(n - 1);
                    return (pow(1 - y, 2) + 100 * pow(x - y * y, 2)).sum();
                  }},
                 {Options::FunctionName::rastrigin,
                  [](Eigen::VectorXd x) {
                    auto x2 = x.array().pow(2) - 10 * cos(2 * M_PI * x.array());
                    return 10 * x.size() + x2.sum();
                  }},
                 {Options::FunctionName::himmelblau, [](Eigen::VectorXd _x) {
                    auto x = _x[0];
                    auto y = _x[1];
                    return std::pow(x * x + y - 11, 2) +
                           std::pow(x + y * y - 7, 2);
                  }}};

namespace run {
/**
  @brief Optimizes function with given options
  @param options Optimization options
  @returns State of optimization after StopCriterion condition has been met
*/
Optimization::State run(Options options);
}  // namespace run

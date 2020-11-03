#pragma once

#include <Eigen/Dense>
#include <functional>
#include <iostream>
#include <memory>
#include <variant>
#include <vector>
#include "mymath.hpp"

namespace Optimization {
/**
 * @brief State struct
 */
struct State {
  size_t iteration_num = 0;  ///< Current iteration number
  size_t iteration_no_improv =
      std::numeric_limits<size_t>::max();  ///< Number of iterations since last
                                           ///< improvement in function minimum
  std::vector<MyMath::PointVal>
      point_history;  ///< Vector of best points at each iteration
};
}  // namespace Optimization

std::ostream& operator<<(std::ostream& os, const Optimization::State& p);

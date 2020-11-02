#pragma once

#include <Eigen/Dense>
#include <functional>
#include <iostream>
#include <memory>
#include <variant>
#include <vector>
#include "mymath.hpp"

namespace Optimization {

struct State {
  size_t iteration_num = 0;
  size_t iteration_no_improv = std::numeric_limits<size_t>::max();
  std::vector<MyMath::PointVal> point_history;
};
}  // namespace Optimization

std::ostream& operator<<(std::ostream& os, const Optimization::State& p);

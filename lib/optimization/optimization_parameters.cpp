#include <optimization_parameters.hpp>

std::ostream&
operator<<(std::ostream& os, const Optimization::OptimizationParameters& p)
{
  os << "num=" << p.iteration_num
     << ", last improv num=" << p.iteration_last_improvement_num
     << ", current_point=(";

  // for (size_t i = 0; i < p.point_history.back().size(); ++i ) {
  //    os << p.point_history.back()[i] << ", ";
  //}
  return os << ")" << std::endl;
}

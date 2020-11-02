#include <optimization_parameters.hpp>

std::ostream&
operator<<(std::ostream& os, const Optimization::OptimizationState& p)
{
  using MyMath::operator<<;
  os << "# of iterations:                 " << p.iteration_num << "\n"
     << "# of iterations w/o improvement: " << p.iteration_no_improv << "\n"
     << "Result: " << p.point_history.back();

  return os;
}

std::ostream&
operator<<(std::ostream& os,
           const Optimization::Parameters::ParametersVariant p)
{
  return os << "()" << std::endl;
}

#include <method.hpp>

#include <Eigen/Dense>
#include <algorithm>
#include <cassert>
#include <iostream>
#include <memory>
#include <numeric>
#include <optimization_parameters.hpp>

Optimization::Method::NelderMead::NelderMead(
  std::shared_ptr<OptimizationParameters> p)
  : Optimization::Method::AbstractMethod(p)
{
  auto pp = std::dynamic_pointer_cast<NelderMeadOptimizationParameters>(p);
  pp->simplex.push_back(createPoint(p->initial_point, parameters->function));

  for (size_t i = 0; i < pp->initial_point.size(); ++i) {
    auto next_point = pp->initial_point;
    next_point[i] += pp->initial_simplex_step;
    pp->simplex.push_back(createPoint(next_point, parameters->function));
  }
  parameters = pp;
};

double
Optimization::Method::NelderMead::next()
{

  auto parameters = std::dynamic_pointer_cast<NelderMeadOptimizationParameters>(
    this->parameters);

  if (!parameters)
    exit(1);

  double alpha = 1;
  double beta = 0.5;
  double gamma = 2;

  std::nth_element(parameters->simplex.begin(),
                   parameters->simplex.begin() + 1,
                   parameters->simplex.end(),
                   PairSecondCmp<VectorXd, double>());
  auto g = parameters->simplex.begin() + 1;

  auto [l, h] = std::minmax_element(parameters->simplex.begin(),
                                    parameters->simplex.end(),
                                    PairSecondCmp<VectorXd, double>());

  // std::cout << l->second << " " << g->second << " " << h->second <<
  // std::endl;

  VectorXd centroid_x =
    VectorXd::Zero(parameters->simplex[0].first.size()).eval();
  for (auto x : parameters->simplex) {
    if (x.first == h->first)
      continue;
    centroid_x += x.first;
  }
  centroid_x /= parameters->simplex.size() - 1;
  // std::cout << "Centroid = " << centroid_x << std::endl;

  auto r = createPoint((1 + alpha) * centroid_x - alpha * h->first,
                       parameters->function);

  if (r.second < l->second) {
    auto e = createPoint((1 - gamma) * centroid_x + gamma * r.first,
                         parameters->function);

    if (e.second < r.second) {
      *h = e;
    } else {
      *h = r;
    }
  } else if ((l->second < r.second) && (r.second < g->second)) {
    *h = r;
  } else {
    if ((g->second < r.second) && (r.second < h->second)) {
      std::swap(*h, r);
    } else if (h->second < r.second) {
      // GOTO 6
    }

    auto s = createPoint(beta * h->first + (1 - beta) * centroid_x,
                         parameters->function);

    if (s.second < h->second) {
      *h = s;
    } else {
      for (auto& x : parameters->simplex) {
        if (x.first == l->first)
          continue;
        x.first = l->first + (x.first - l->first) / 2;
        x.second = parameters->function(x.first);
      }
    }
  }

  parameters->iteration_num += 1;
  auto rr = *std::min_element(parameters->simplex.begin(),
                              parameters->simplex.end(),
                              PairSecondCmp<VectorXd, double>());
  std::cout << rr.first << std::endl << std::endl;
  std::cout << "f = " << rr.second << std::endl << std::endl;
  return rr.second;
};

PointVal
Optimization::Method::NelderMead::createPoint(
  Eigen::VectorXd v,
  std::function<double(VectorXd)>& f)
{
  return PointVal(v, f(v));
}

Optimization::Method::RandomSearch::RandomSearch(
  std::shared_ptr<OptimizationParameters> p)
  : Optimization::Method::AbstractMethod(p)
{
  return;
}

double
Optimization::Method::RandomSearch::next()
{
  return 0;
}

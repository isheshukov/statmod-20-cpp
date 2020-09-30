#include <method.hpp>

#include <Eigen/Dense>
#include <algorithm>
#include <iostream>
#include <memory>
#include <numeric>
#include <optimization_parameters.hpp>

Optimization::Method::NelderMead::NelderMead(
  std::shared_ptr<OptimizationParameters> p)
  : Optimization::Method::AbstractMethod(p)
{
  p->simplex.push_back(std::pair<VectorXd, double>(
    p->initial_point, p->function(p->initial_point)));

  for (size_t i = 0; i < p->initial_point.size(); ++i) {
    auto next_point = p->initial_point;
    next_point[i] += p->initial_simplex_step;
    p->simplex.push_back(
      std::pair<VectorXd, double>(next_point, p->function(next_point)));
  }
  parameters = p;
};

double
Optimization::Method::NelderMead::next()
{

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

  auto r = std::pair<VectorXd, double>(
    (1 + alpha) * centroid_x - alpha * h->first,
    parameters->function((1 + alpha) * centroid_x - alpha * h->first));

  if (r.second < l->second) {
    auto e = std::pair<VectorXd, double>(
      (1 - gamma) * centroid_x + gamma * r.first,
      parameters->function((1 - gamma) * centroid_x + gamma * r.first));

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

    auto s = std::pair<VectorXd, double>(
      beta * h->first + (1 - beta) * centroid_x,
      parameters->function(beta * h->first + (1 - beta) * centroid_x));

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
  return rr.second;
};

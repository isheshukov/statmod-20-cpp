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
  simplex.push_back(std::pair<VectorXd, double>(p->initial_point,
                                                p->function(p->initial_point)));

  for (size_t i = 0; i < p->initial_point.size(); ++i) {
    auto next_point = p->initial_point;
    next_point[i] += initial_simplex_step;
    simplex.push_back(
      std::pair<VectorXd, double>(next_point, p->function(next_point)));
  }
};

void
Optimization::Method::NelderMead::next()
{

  std::nth_element(simplex.begin(),
                   simplex.begin() + 1,
                   simplex.end(),
                   PairSecondCmp<VectorXd, double>());
  auto g = simplex.begin() + 1;

  auto [l, h] = std::minmax_element(
    simplex.begin(), simplex.end(), PairSecondCmp<VectorXd, double>());

  std::cout << l->second << " " << g->second << " " << h->second << std::endl;

  VectorXd centroid_x = VectorXd::Zero(simplex[0].first.size()).eval();
  for (auto x : simplex) {
    if (x.first == h->first)
      continue;
    centroid_x += x.first;
  }
  centroid_x /= simplex.size();
  std::cout << "Centroid = " << centroid_x << std::endl;

  double alpha = 1;
  double gamma = 2;
  double beta = 0.5;

  auto r = std::pair<VectorXd, double>(
    (1 + 1) * centroid_x - 1 * h->first,
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
      for (auto& x : simplex) {
        if (x.first == l->first)
          continue;
        x.first = l->first + (x.first - l->first) / 2;
        x.second = parameters->function(x.first);
      }
    }
  }

  parameters->iteration_num += 1;
  std::cout << *parameters;
};

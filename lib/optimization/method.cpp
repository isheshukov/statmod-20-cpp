#include <method.hpp>

#include "mymath.hpp"
#include <Eigen/Dense>
#include <algorithm>
#include <cassert>
#include <iostream>
#include <memory>
#include <numeric>
#include <optimization_parameters.hpp>

std::random_device r;
std::mt19937 eng(r());

Optimization::Method::NelderMead::NelderMead(
  std::shared_ptr<OptimizationParameters> p)
  : Optimization::Method::AbstractMethod(p)
{
  auto pp = std::dynamic_pointer_cast<NelderMeadOptimizationParameters>(p);
  pp->simplex.push_back(
    MyMath::createPointVal(p->initial_point, parameters->function));

  for (long i = 0; i < pp->initial_point.size(); ++i) {
    auto next_point = pp->initial_point;
    next_point[i] += pp->initial_simplex_step;
    pp->simplex.push_back(
      MyMath::createPointVal(next_point, parameters->function));
  }
  parameters = pp;
};

MyMath::PointVal
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
                   PairSecondCmp<Eigen::VectorXd, double>());
  auto g = parameters->simplex.begin() + 1;

  auto [l, h] = std::minmax_element(parameters->simplex.begin(),
                                    parameters->simplex.end(),
                                    PairSecondCmp<Eigen::VectorXd, double>());

  auto centroid_x =
    Eigen::VectorXd::Zero(parameters->simplex[0].first.size()).eval();
  for (auto x : parameters->simplex) {
    if (x.first == h->first)
      continue;
    centroid_x += x.first;
  }
  centroid_x /= parameters->simplex.size() - 1;

  auto r = MyMath::createPointVal((1 + alpha) * centroid_x - alpha * h->first,
                                  parameters->function);

  if (r.second < l->second) {
    auto e = MyMath::createPointVal((1 - gamma) * centroid_x + gamma * r.first,
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

    auto s = MyMath::createPointVal(beta * h->first + (1 - beta) * centroid_x,
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

  auto rr = *std::min_element(parameters->simplex.begin(),
                              parameters->simplex.end(),
                              PairSecondCmp<Eigen::VectorXd, double>());
  return rr;
};

Optimization::Method::RandomSearch::RandomSearch(
  std::shared_ptr<OptimizationParameters> p)
  : Optimization::Method::AbstractMethod(p)
{
  auto pp = std::dynamic_pointer_cast<RandomSearchOptimizationParameters>(p);
  parameters = pp;
}

MyMath::PointVal
Optimization::Method::RandomSearch::next()
{
  auto parameters =
    std::dynamic_pointer_cast<RandomSearchOptimizationParameters>(
      this->parameters);

  auto bernoulli_dist = std::bernoulli_distribution(parameters->p);
  MyMath::Box search_space;
  MyMath::PointVal candidate;
  double new_delta = parameters->delta;

  if (bernoulli_dist(eng)) {
    search_space = parameters->search_space;
  } else {
    search_space =
      MyMath::Box(parameters->current_best.first, parameters->delta) &
      parameters->search_space;
    new_delta = parameters->alpha * parameters->delta;
  }
  candidate = MyMath::createPointVal(MyMath::sample(search_space, eng),
                                     parameters->function);

  if (candidate.second < parameters->current_best.second) {
    parameters->current_best = candidate;
    parameters->delta = new_delta;
  }

  return parameters->current_best;
}

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

void
Optimization::Method::NelderMead::setParameters(
  const Optimization::Parameters::NelderMead& p)
{
  this->parameters = p;
  this->parameters.simplex.push_back(
    MyMath::createPointVal(p.initial_point, this->parameters.function));

  for (long i = 0; i < p.initial_point.size(); ++i) {
    auto next_point = p.initial_point;
    next_point[i] += p.initial_simplex_step;
    this->parameters.simplex.push_back(
      MyMath::createPointVal(next_point, this->parameters.function));
  }
};

MyMath::PointVal
Optimization::Method::NelderMead::next()
{
  double alpha = 1;
  double beta = 0.5;
  double gamma = 2;

  std::nth_element(this->parameters.simplex.begin(),
                   this->parameters.simplex.begin() + 1,
                   this->parameters.simplex.end(),
                   PairSecondCmp<Eigen::VectorXd, double>());
  auto g = this->parameters.simplex.begin() + 1;

  auto [l, h] = std::minmax_element(this->parameters.simplex.begin(),
                                    this->parameters.simplex.end(),
                                    PairSecondCmp<Eigen::VectorXd, double>());

  auto centroid_x =
    Eigen::VectorXd::Zero(this->parameters.simplex[0].first.size()).eval();
  for (auto x : this->parameters.simplex) {
    if (x.first == h->first)
      continue;
    centroid_x += x.first;
  }
  centroid_x /= this->parameters.simplex.size() - 1;

  auto r = MyMath::createPointVal((1 + alpha) * centroid_x - alpha * h->first,
                                  this->parameters.function);

  if (r.second < l->second) {
    auto e = MyMath::createPointVal((1 - gamma) * centroid_x + gamma * r.first,
                                    this->parameters.function);

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
                                    this->parameters.function);

    if (s.second < h->second) {
      *h = s;
    } else {
      for (auto& x : this->parameters.simplex) {
        if (x.first == l->first)
          continue;
        x.first = l->first + (x.first - l->first) / 2;
        x.second = this->parameters.function(x.first);
      }
    }
  }

  auto rr = *std::min_element(this->parameters.simplex.begin(),
                              this->parameters.simplex.end(),
                              PairSecondCmp<Eigen::VectorXd, double>());
  return rr;
};

void
Optimization::Method::RandomSearch::setParameters(
  const Optimization::Parameters::RandomSearch& p)
{
  parameters = p;
}

MyMath::PointVal
Optimization::Method::RandomSearch::next()
{
  auto bernoulli_dist = std::bernoulli_distribution(this->parameters.p);
  MyMath::Box search_space;
  MyMath::PointVal candidate;
  double new_delta = this->parameters.delta;

  if (bernoulli_dist(eng)) {
    search_space = this->parameters.search_space;
  } else {
    search_space =
      MyMath::Box(this->parameters.current_best.first, this->parameters.delta) &
      this->parameters.search_space;
    new_delta = this->parameters.alpha * this->parameters.delta;
  }
  candidate = MyMath::createPointVal(MyMath::sample(search_space, eng),
                                     this->parameters.function);

  if (candidate.second < this->parameters.current_best.second) {
    this->parameters.current_best = candidate;
    this->parameters.delta = new_delta;
  }

  return this->parameters.current_best;
}

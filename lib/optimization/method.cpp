#include <method.hpp>

#include <Eigen/Dense>
#include <algorithm>
#include <cassert>
#include <iostream>
#include <memory>
#include <numeric>
#include <optimization_parameters.hpp>
#include "mymath.hpp"

std::random_device r;
std::mt19937 eng(r());

Optimization::Method::NelderMead::NelderMead(
    std::function<double(Eigen::VectorXd)> _function,
    MyMath::Box _search_space,
    Eigen::VectorXd _initial_point,
    double _initial_simplex_step)
    : function(_function),
      search_space(_search_space),
      initial_point(_initial_point),
      current_best(MyMath::createPointVal(initial_point, function)),
      initial_simplex_step(_initial_simplex_step) {
  this->simplex.push_back(
      MyMath::createPointVal(_initial_point, this->function));

  for (long i = 0; i < _initial_point.size(); ++i) {
    auto next_point = _initial_point;
    next_point[i] += _initial_simplex_step;
    this->simplex.push_back(MyMath::createPointVal(next_point, this->function));
  }
};

MyMath::PointVal Optimization::Method::NelderMead::next() {
  double alpha = 1;
  double beta = 0.5;
  double gamma = 2;

  std::nth_element(this->simplex.begin(), this->simplex.begin() + 1,
                   this->simplex.end(),
                   PairSecondCmp<Eigen::VectorXd, double>());
  auto g = this->simplex.begin() + 1;

  auto [l, h] = std::minmax_element(this->simplex.begin(), this->simplex.end(),
                                    PairSecondCmp<Eigen::VectorXd, double>());

  auto centroid_x = Eigen::VectorXd::Zero(this->simplex[0].first.size()).eval();
  for (auto x : this->simplex) {
    if (x.first == h->first)
      continue;
    centroid_x += x.first;
  }
  centroid_x /= this->simplex.size() - 1;

  auto r = MyMath::createPointVal((1 + alpha) * centroid_x - alpha * h->first,
                                  this->function);

  if (r.second < l->second) {
    auto e = MyMath::createPointVal((1 - gamma) * centroid_x + gamma * r.first,
                                    this->function);

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
                                    this->function);

    if (s.second < h->second) {
      *h = s;
    } else {
      for (auto& x : this->simplex) {
        if (x.first == l->first)
          continue;
        x.first = l->first + (x.first - l->first) / 2;
        x.second = this->function(x.first);
      }
    }
  }

  auto rr = *std::min_element(this->simplex.begin(), this->simplex.end(),
                              PairSecondCmp<Eigen::VectorXd, double>());
  this->current_best = rr;

  return rr;
};

Optimization::Method::RandomSearch::RandomSearch(
    std::function<double(Eigen::VectorXd)> _function,
    MyMath::Box _search_space,
    Eigen::VectorXd _initial_point,
    double _p,
    double _delta,
    double _alpha)
    : function(_function),
      search_space(_search_space),
      initial_point(_initial_point),
      current_best(MyMath::createPointVal(initial_point, function)),
      p(_p),
      delta(_delta),
      alpha(_alpha) {}

MyMath::PointVal Optimization::Method::RandomSearch::next() {
  auto bernoulli_dist = std::bernoulli_distribution(this->p);
  MyMath::Box search_space;
  MyMath::PointVal candidate;
  double new_delta = this->delta;

  if (bernoulli_dist(eng)) {
    search_space = this->search_space;
  } else {
    search_space =
        MyMath::Box(this->current_best.first, this->delta) & this->search_space;
    new_delta = this->alpha * this->delta;
  }
  candidate =
      MyMath::createPointVal(MyMath::sample(search_space, eng), this->function);

  if (candidate.second < this->current_best.second) {
    this->current_best = candidate;
    this->delta = new_delta;
  }

  return this->current_best;
}

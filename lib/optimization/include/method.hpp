#pragma once

#include <optimization_parameters.hpp>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <Eigen/Dense>

template <class U, class V>
struct PairSecondCmp{
    bool operator()(const std::pair<U, V>& a, const std::pair<U, V>& b){
        return a.second < b.second;
    }
};

namespace Optimization {
namespace Method {
class OptimizationParameters;
class AbstractMethod {
public:
    AbstractMethod(OptimizationParameters* p) {
        this->parameters = p;
    }
    virtual void next() {}
    OptimizationParameters* parameters;
};

class NelderMead : public AbstractMethod {
    std::vector<std::pair<Eigen::VectorXd, double>> simplex;
    double initial_simplex_step = 1.0;
public:
    NelderMead(OptimizationParameters* p);
    virtual void next() override;
};

}}

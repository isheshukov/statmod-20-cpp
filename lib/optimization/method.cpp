#include <method.hpp>

#include <optimization_parameters.hpp>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <Eigen/Dense>

Optimization::Method::NelderMead::NelderMead(OptimizationParameters& p) : Optimization::Method::AbstractMethod(p) {
        simplex.push_back(std::pair<VectorXd, double>(p.initial_point, p.function(p.initial_point)));

        for (size_t i = 0; i < p.initial_point.size(); ++i){
            auto next_point = p.initial_point;
            next_point[i] += initial_simplex_step;
            simplex.push_back(std::pair<VectorXd, double>(next_point, p.function(next_point)));
        }

    };

void Optimization::Method::NelderMead::next() {

        auto [l, h] = std::minmax_element(simplex.begin(), simplex.end(), PairSecondCmp<VectorXd, double>());


        std::nth_element(simplex.begin(), simplex.begin()+1, simplex.end(), PairSecondCmp<VectorXd, double>());
        auto g = simplex.begin() + 1;

        std::cout << l->second << " " << g->second << " " << h->second << std::endl;


        VectorXd centroid_x = VectorXd::Zero(simplex[0].first.size()).eval();
        for (auto x : simplex) {
            centroid_x += x.first;
        }
        centroid_x /= simplex.size() - 1;

        std::cout << centroid_x << std::endl;

        parameters.iteration_num += 1;
        std::cout << parameters;

        //parameters.point_history.push_back(
        //  parameters.point_history.back()
        //);
    };

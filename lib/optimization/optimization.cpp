#include <optimization.hpp>
#include <optimization_parameters.hpp>


namespace Optimization {
void optimize(OptimizationParameters* p) {
    do {
        p->method->next();
    } while (p->stop_criterion->check(*p));
}
}

#pragma once

#include <Eigen/Dense>
#include <iostream>
#include <random>
#include <vector>

using namespace Eigen;

namespace MyMath {
using PointVal = std::pair<Eigen::VectorXd, double>;

class Box
{
public:
  Box();
  Box(std::vector<std::pair<double, double>>);
  Box(Eigen::VectorXd& center, double radius);
  size_t size() const;
  double measure() const;
  const std::pair<double, double>& operator[](size_t n) const;

private:
  std::vector<std::pair<double, double>> m_points;
};

Box operator&(const Box& a, const Box& b);
std::ostream&
operator<<(std::ostream& os, const Box& box);
std::ostream&
operator<<(std::ostream& os, const PointVal& box);

template<class URBG>
VectorXd
sample(Box& box, URBG&& g)
{
  VectorXd result(box.size());
  for (size_t i = 0; i < box.size(); ++i) {
    std::uniform_real_distribution<> d(box[i].first, box[i].second);
    result[i] = d(g);
  }
  return result;
}

PointVal
createPointVal(Eigen::VectorXd v, std::function<double(VectorXd)>& f);

}

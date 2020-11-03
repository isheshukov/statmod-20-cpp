#pragma once

#include <Eigen/Dense>
#include <iostream>
#include <random>
#include <vector>

namespace MyMath {
using PointVal = std::pair<Eigen::VectorXd, double>;

/**
 * @brief The Box class
 *
 * Hyperrectangle class
 */

class Box
{
public:
  Box();
  Box(const std::vector<std::pair<double, double>>&);
  Box(const Eigen::VectorXd& center, double radius);
  size_t size() const;
  /**
   * @brief Measure function
   * @return Measure of a box
   */
  double measure() const;
  /**
   * @brief operator[]
   * @return Pair @f$(x^{(i)}_1, x^{(i)}_2)@f$ , start and end of i-rd
   * coordinate.
   */
  const std::pair<double, double>& operator[](size_t n) const;

private:
  std::vector<std::pair<double, double>> m_points;
};

Box
operator&(const Box& a, const Box& b);
std::ostream&
operator<<(std::ostream& os, const Box& box);
std::ostream&
operator<<(std::ostream& os, const PointVal& box);

template<class URBG>
Eigen::VectorXd
sample(Box& box, URBG&& g)
{
  Eigen::VectorXd result(box.size());
  for (size_t i = 0; i < box.size(); ++i) {
    std::uniform_real_distribution<> d(box[i].first, box[i].second);
    result[i] = d(g);
  }
  return result;
}

PointVal
createPointVal(Eigen::VectorXd v, std::function<double(Eigen::VectorXd)>& f);

}

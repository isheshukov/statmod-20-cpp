#include <Eigen/Dense>
#include <cassert>
#include <iostream>
#include <mymath.hpp>
#include <numeric>
#include <random>
#include <vector>

using dpair = std::pair<double, double>;

MyMath::Box::Box() {}

MyMath::Box::Box(std::vector<dpair> p)
{
  this->m_points = p;
}

MyMath::Box::Box(Eigen::VectorXd& center, double radius)
{
  std::vector<dpair> points(center.size());
  for (size_t i = 0; i < points.size(); ++i) {
    points[i] = dpair(center[i] - radius, center[i] + radius);
  }
  this->m_points = std::move(points);
}

size_t
MyMath::Box::size() const
{
  return this->m_points.size();
}

double
MyMath::Box::measure() const
{
  return std::accumulate(this->m_points.begin(),
                         this->m_points.end(),
                         dpair(1, std::numeric_limits<double>::min()),
                         [](const dpair& acc, const dpair& value) {
                           return dpair((value.second - value.first) *
                                          acc.first,
                                        std::numeric_limits<double>::min());
                         })
    .first;
}

const std::pair<double, double>& MyMath::Box::operator[](size_t n) const
{
  return this->m_points[n];
}

MyMath::Box MyMath::operator&(const MyMath::Box& a, const MyMath::Box& b)
{
  assert(a.size() == b.size() && "Boxes dimensions must be equal");
  size_t size = a.size();
  std::vector<dpair> pairs(size);

  for (size_t i = 0; i < size; ++i) {
    pairs[i] = dpair(std::max(a[i].first, b[i].first),
                     std::min(a[i].second, b[i].second));
  }

  return MyMath::Box(pairs);
}

std::ostream&
MyMath::operator<<(std::ostream& os, const Box& box)
{
  for (size_t i = 0; i < box.size(); ++i) {
    os << "x_" << i << " (" << box[i].first << "," << box[i].second << ")"
       << std::endl;
  }
  return os;
}

std::ostream&
MyMath::operator<<(std::ostream& os, const MyMath::PointVal& x)
{
  return os << "x = (" << x.first.transpose() << "), f = " << x.second;
}

MyMath::PointVal
MyMath::createPointVal(Eigen::VectorXd v, std::function<double(VectorXd)>& f)
{
  return PointVal(v, f(v));
}

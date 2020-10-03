#include <cassert>
#include <mymath.hpp>
#include <numeric>

using MyMath::Box;

using dpair = std::pair<double, double>;

Box::Box(std::vector<dpair> p)
{
  this->m_points = p;
}

size_t
Box::size() const
{
  return this->m_points.size();
}

double
Box::measure() const
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

const std::pair<double, double>& Box::operator[](size_t n) const
{
  return this->m_points[n];
}

Box MyMath::operator&(const Box& a, const Box& b)
{
  assert(a.size() == b.size() && "Boxes dimensions must be equal");
  size_t size = a.size();
  std::vector<dpair> pairs(size);

  for (size_t i = 0; i < size; ++i) {
    pairs[i] = dpair(std::max(a[i].first, b[i].first),
                     std::min(a[i].second, b[i].second));
  }

  return Box(pairs);
}

std::ostream&
MyMath::operator<<(std::ostream& os, const Box& box)
{
  for (size_t i = 0; i < box.size(); ++i) {
    os << "x_" << i << " (" << box[i].first << "," << box[i].second << ")"
       << std::endl;
  }
  return os;
};

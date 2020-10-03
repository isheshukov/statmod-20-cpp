#include <iostream>
#include <vector>

namespace MyMath {
class Box
{
public:
  Box();
  Box(std::vector<std::pair<double, double>>);
  size_t size() const;
  double measure() const;
  const std::pair<double, double>& operator[](size_t n) const;

private:
  std::vector<std::pair<double, double>> m_points;
};

Box operator&(const Box& a, const Box& b);
std::ostream&
operator<<(std::ostream& os, const Box& box);

}

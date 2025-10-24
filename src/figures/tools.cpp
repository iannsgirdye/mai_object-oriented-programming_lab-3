#include "../../include/figures.hpp"
#include <vector>
#include <cmath>


#define EPSILON 1e-6


struct Point _summand(const struct Point p1, const struct Point p2) {
  struct Point result;
  result.x = (p1.x + p2.x) * (p1.x * p2.y - p2.x * p1.y);
  result.y = (p1.y + p2.y) * (p1.x * p2.y - p2.x * p1.y);
  return result;
}


struct Point Figure::center() const {
  if (points.empty()) {
    throw Figure::EmptyFigureException();
  }

  const double S = square();
  struct Point c = {0.0, 0.0};

  if (fabs(S - 0.0) < EPSILON) {
    return c;
  }

  for (size_t i = 0; i != points.size(); ++i) {
    struct Point summand = _summand(points[i], points[(i + 1) % points.size()]);
    c.x += summand.x;
    c.y += summand.y;
  }

  const double k = 1.0 / (6 * square());
  c.x *= k;
  c.y *= k;

  return c;
}


double _side(const struct Point p1, const struct Point p2) {
  const double sideX = p1.x - p2.x;
  const double sideY = p1.y - p2.y;
  return sqrt(sideX * sideX + sideY * sideY);
}


double Figure::square() const {
  if (points.empty()) {
    throw Figure::EmptyFigureException();
  }

  const double side = _side(points[0], points[1]);
  const double tg = tan(M_PI / points.size());
  return (points.size() * side * side) / (4 * tg);
}

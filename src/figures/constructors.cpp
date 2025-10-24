#include "../../include/figures.hpp"
#include <vector>


Figure::Figure(const pointsCountFigure pC) {
  points.resize(static_cast<size_t>(pC));
  for (size_t i = 0; i != points.size(); ++i) {
    points[i] = {0.0, 0.0};
  }
}


Figure::Figure(const std::vector<struct Point> &ps, const pointsCountFigure psC) {
  if (ps.size() != static_cast<size_t>(psC)) {
    throw IncorrectPointsCountException();
  }
  points = ps;
}


Triangle::Triangle(): Figure(pointsCountFigure::triangle) {}
Triangle::Triangle(const std::vector<struct Point> &ps): Figure(ps, pointsCountFigure::triangle) {}


Hexagon::Hexagon(): Figure(pointsCountFigure::hexagon) {}
Hexagon::Hexagon(const std::vector<struct Point> &ps): Figure(ps, pointsCountFigure::hexagon) {}


Octagon::Octagon(): Figure(pointsCountFigure::octagon) {}
Octagon::Octagon(const std::vector<struct Point> &ps): Figure(ps, pointsCountFigure::octagon) {}

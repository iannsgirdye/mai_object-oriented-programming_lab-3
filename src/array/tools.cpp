#include "../../include/array.hpp"
#include "../../include/figures.hpp"
#include <iostream>


void FiguresArray::printCenters() const {
  struct Point c;
  for (size_t i = 0; i != figures.size(); ++i) {
    c = figures[i]->center();
    std::cout << "(" << c.x << ", " << c.y << ")" << std::endl;
  }
}


void FiguresArray::printSquares() const {
  for (size_t i = 0; i != figures.size(); ++i) {
    std::cout << figures[i]->square() << std::endl;
  }
}


double FiguresArray::sumOfSquares() const {
  double _sum = 0;
  for (size_t i = 0; i != figures.size(); ++i) {
    _sum += figures[i]->square();
  }
  return _sum;
}


size_t FiguresArray::getSize() const {
  return figures.size();
}


void FiguresArray::pushBack(const Figure *f) {
  figures.push_back(f);
}


void FiguresArray::remove(const size_t i) {
  if (i >= figures.size()) {
    throw InvalidIndexException();
  }
  figures.erase(figures.begin() + i);
}

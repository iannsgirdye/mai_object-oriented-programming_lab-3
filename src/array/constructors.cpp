#include "../../include/array.hpp"
#include "../../include/figures.hpp"
#include <vector>


FiguresArray::FiguresArray(const size_t &n, const Figure *f) {
  for (size_t i = 0; i != n; ++i) {
    figures.push_back(f);
  }
}


FiguresArray::FiguresArray(const std::initializer_list<const Figure *> &fs) {
  figures = fs;
}


FiguresArray::FiguresArray(const std::vector<const Figure *> &fs) {
  figures = fs;
}


FiguresArray::FiguresArray(const FiguresArray &other) {
  for (size_t i = 0; i != other.figures.size(); ++i) {
    figures.push_back(other.figures[i]);
  }
}


FiguresArray::FiguresArray(FiguresArray &&other) noexcept {
  figures = other.figures;
}

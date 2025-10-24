#ifndef ARRAY_HPP
#define ARRAY_HPP


#include "./figures.hpp"
#include <vector>


class FiguresArray {
  private:
    std::vector<const Figure *> figures;
  
  public:
    FiguresArray() = default;
    FiguresArray(const size_t &n, const Figure *f = nullptr);
    FiguresArray(const std::initializer_list<const Figure *> &fs);
    FiguresArray(const std::vector<const Figure *> &fs);
    FiguresArray(const FiguresArray &other);
    FiguresArray(FiguresArray &&other) noexcept;

    void printCenters() const;
    void printSquares() const;
    double sumOfSquares() const;

    size_t getSize() const;
    void pushBack(const Figure *f);
    void remove(const size_t i);

    class InvalidIndexException {};
};


#endif

#include <gtest/gtest.h>
#include <sstream>
#include <cmath>
#include <iostream>
#include "./include/figures.hpp"
#include "./include/array.hpp"

constexpr double EPS = 1e-6;

// --- Вспомогательная функция для создания правильного n-угольника ---
// Создаёт набор вершин правильного многоугольника с радиусом описанной окружности R
// и центром в точке (cx, cy)
static std::vector<Point> make_regular_polygon(size_t n, double R, double cx = 0.0, double cy = 0.0) {
  std::vector<Point> pts(n);
  for (size_t i = 0; i < n; ++i) {
    double ang = 2.0 * M_PI * i / n;
    pts[i] = { cx + R * std::cos(ang), cy + R * std::sin(ang) };
  }
  return pts;
}

// ---------------------- TESTS: FIGURES ----------------------

TEST(FigureTest, DefaultConstructorsInitializeToZero) {
    // Проверяем, что конструкторы по умолчанию создают "пустые" фигуры —
    // все вершины в начале координат, центр в (0,0), площадь равна 0.
    Triangle t;
    Hexagon h;
    Octagon o;

    std::vector<const Figure*> figs = { &t, &h, &o };
    for (auto f : figs) {
        Point c = f->center();
        EXPECT_NEAR(c.x, 0.0, EPS);  // центр X = 0
        EXPECT_NEAR(c.y, 0.0, EPS);  // центр Y = 0
        EXPECT_NEAR(f->square(), 0.0, EPS);  // площадь = 0
    }
}

TEST(FigureTest, TriangleSquareAndCenterAreCorrect) {
    // Проверяем, что для правильного треугольника методы square() и center()
    // возвращают корректные значения площади и центра.
    double a = 2.0;                     // сторона треугольника
    double R = a / std::sqrt(3.0);      // радиус описанной окружности
    auto pts = make_regular_polygon(3, R); // создаём правильный треугольник
    Triangle t(pts);

    // Ожидаемая площадь правильного треугольника
    double expected_area = std::sqrt(3.0) / 4.0 * a * a;
    EXPECT_NEAR(t.square(), expected_area, EPS); // сравнение площади

    // Центр правильного треугольника должен находиться в (0,0)
    Point c = t.center();
    EXPECT_NEAR(c.x, 0.0, EPS);
    EXPECT_NEAR(c.y, 0.0, EPS);
}

TEST(FigureTest, HexagonSquareAndCenterAreCorrect) {
  // Проверяем, что для правильного шестиугольника центр в (0,0),
  // а площадь положительная (ненулевая).
  auto pts = make_regular_polygon(6, 1.0);
  Hexagon h(pts);
  EXPECT_NEAR(h.center().x, 0.0, EPS);
  EXPECT_NEAR(h.center().y, 0.0, EPS);
  EXPECT_GT(h.square(), 0.0);  // площадь > 0
}

TEST(FigureTest, OctagonSquareAndCenterAreCorrect) {
  // Проверяем аналогично для правильного восьмиугольника.
  auto pts = make_regular_polygon(8, 2.0);
  Octagon o(pts);
  EXPECT_NEAR(o.center().x, 0.0, EPS);
  EXPECT_NEAR(o.center().y, 0.0, EPS);
  EXPECT_GT(o.square(), 0.0);
}

TEST(FigureTest, EqualityOperatorDetectsTranslation) {
  // Проверяем работу оператора ==:
  // две фигуры считаются равными, если одна является сдвигом другой.
  auto pts = make_regular_polygon(6, 1.0, 0.0, 0.0);
  Hexagon h1(pts);

  // Сдвигаем каждую вершину на (5, -3)
  for (auto &p : pts) { p.x += 5.0; p.y -= 3.0; }
  Hexagon h2(pts);

  // Должны быть равны, т.к. форма и размер одинаковы
  EXPECT_TRUE(h1 == h2);
}

TEST(FigureTest, OutputOperatorPrintsCoordinates) {
  // Проверяем работу оператора вывода (<<):
  // в потоке должны появляться скобки и координаты вершин.
  Triangle t({ {0,0}, {1,0}, {0,1} });
  std::ostringstream oss;
  std::streambuf* oldbuf = std::cout.rdbuf();
  std::cout.rdbuf(oss.rdbuf());  // перенаправляем вывод
  std::cout << t;
  std::cout.rdbuf(oldbuf);       // возвращаем cout
  std::string out = oss.str();
  EXPECT_NE(out.find("("), std::string::npos);
  EXPECT_NE(out.find(")"), std::string::npos);
}

TEST(FigureTest, InputOperatorReadsCoordinates) {
  // Проверяем работу оператора ввода (>>):
  // фигура, считанная из потока, должна совпадать с эталонной.
  Triangle expected({ {0,0}, {1,0}, {0,1} });
  Triangle t;

  // Формируем входные данные (три пары координат)
  std::istringstream iss("0 0 1 0 0 1");
  std::streambuf* oldcin = std::cin.rdbuf();
  std::cin.rdbuf(iss.rdbuf());
  std::cin >> t;                  // читаем фигуру
  std::cin.rdbuf(oldcin);         // возвращаем cin

  EXPECT_TRUE(t == expected);     // сравниваем с эталонным треугольником
}

// ---------------------- TESTS: FiguresArray ----------------------

TEST(FiguresArrayTest, ConstructorsAndPushBackWork) {
  // Проверяем работу конструктора и метода pushBack():
  // при добавлении элементов размер массива увеличивается.
  Triangle t({ {0,0},{1,0},{0,1} });
  Hexagon h(make_regular_polygon(6, 1.0));

  FiguresArray arr;
  EXPECT_EQ(arr.getSize(), 0u);   // изначально пустой
  arr.pushBack(&t);
  arr.pushBack(&h);
  EXPECT_EQ(arr.getSize(), 2u);   // добавили два элемента
}

TEST(FiguresArrayTest, SumOfSquaresIsCorrect) {
  // Проверяем корректность метода sumOfSquares():
  // сумма площадей элементов массива должна совпадать
  // с арифметической суммой площадей фигур.
  Triangle t({ {0,0},{1,0},{0,1} });
  Hexagon h(make_regular_polygon(6, 1.0));
  FiguresArray arr;
  arr.pushBack(&t);
  arr.pushBack(&h);
  double expected = t.square() + h.square();
  EXPECT_NEAR(arr.sumOfSquares(), expected, EPS);
}

TEST(FiguresArrayTest, RemoveWorksAndThrowsOnInvalidIndex) {
  // Проверяем корректность удаления и выброс исключения при неверном индексе.
  Octagon o(make_regular_polygon(8, 1.0));
  FiguresArray arr;
  arr.pushBack(&o);
  arr.pushBack(&o);
  EXPECT_EQ(arr.getSize(), 2u);
  arr.remove(0);                                  // удаляем первый элемент
  EXPECT_EQ(arr.getSize(), 1u);
  EXPECT_THROW(arr.remove(10), FiguresArray::InvalidIndexException); // индекс вне диапазона
}

TEST(FiguresArrayTest, PrintCentersAndSquaresProduceOutput) {
  // Проверяем, что методы printCenters() и printSquares()
  // выводят информацию в поток (stdout не остаётся пустым).
  Triangle t({ {0,0},{1,0},{0,1} });
  Hexagon h(make_regular_polygon(6, 1.0));
  FiguresArray arr;
  arr.pushBack(&t);
  arr.pushBack(&h);
  std::ostringstream oss;
  std::streambuf* oldbuf = std::cout.rdbuf();
  std::cout.rdbuf(oss.rdbuf());
  arr.printCenters();
  arr.printSquares();
  std::cout.rdbuf(oldbuf);
  std::string out = oss.str();
  EXPECT_FALSE(out.empty());  // должен быть хоть какой-то текст
}

TEST(FiguresArrayTest, CopyAndMoveConstructorsWork) {
  // Проверяем корректность копирования и перемещения массива фигур.
  Triangle t({ {0,0},{1,0},{0,1} });
  FiguresArray arr;
  arr.pushBack(&t);

  // Конструктор копирования
  FiguresArray copy(arr);
  EXPECT_EQ(copy.getSize(), 1u);

  // Конструктор перемещения
  FiguresArray moved(std::move(copy));
  EXPECT_EQ(moved.getSize(), 1u);
}

// ---------------------- TESTS: COPY AND MOVE OPERATORS (FIGURE) ----------------------

TEST(FigureTest, CopyAssignmentOperator_WorksCorrectly) {
  // Создаём правильный треугольник (оригинал)
  auto pts = make_regular_polygon(3, 1.0);
  Triangle t1(pts);

  // Создаём другую фигуру и копируем в неё
  Triangle t2;
  t2 = t1;  // <-- вызывается Figure::operator=(const Figure&)

  // Проверяем, что копия совпадает с оригиналом
  EXPECT_TRUE(t1 == t2);

  // Центры и площади тоже должны совпадать
  EXPECT_NEAR(t1.center().x, t2.center().x, EPS);
  EXPECT_NEAR(t1.center().y, t2.center().y, EPS);
  EXPECT_NEAR(t1.square(), t2.square(), EPS);
}

TEST(FigureTest, MoveAssignmentOperator_WorksCorrectly) {
  // Создаём исходный шестиугольник
  auto pts = make_regular_polygon(6, 1.0);
  Hexagon h1(pts);

  // Создаём другой шестиугольник и перемещаем в него
  Hexagon h2;
  h2 = std::move(h1);  // <-- вызывается Figure::operator=(Figure&&)

  // Проверяем, что данные успешно "переехали"
  EXPECT_GT(h2.square(), 0.0);
  EXPECT_NEAR(h2.center().x, 0.0, EPS);
  EXPECT_NEAR(h2.center().y, 0.0, EPS);

  // Исходный объект должен быть пустым → вызов его методов вызывает исключение
  EXPECT_THROW(h1.center(), Figure::EmptyFigureException);
  EXPECT_THROW(h1.square(), Figure::EmptyFigureException);
}

TEST(FigureTest, SelfAssignment_DoesNotBreakObject) {
  // Проверяем, что самоприсваивание безопасно
  auto pts = make_regular_polygon(8, 1.0);
  Octagon o(pts);

  // Сохраняем текущие значения
  double area_before = o.square();
  Point c_before = o.center();

  // Самоприсваивание
  o = o;  // <-- operator=(const Figure&), но this == &other

  // Проверяем, что объект не изменился
  EXPECT_NEAR(o.square(), area_before, EPS);
  EXPECT_NEAR(o.center().x, c_before.x, EPS);
  EXPECT_NEAR(o.center().y, c_before.y, EPS);
}

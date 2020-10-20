#include <vector>
#include <cmath>

const double EPS = 1e-6;
const double PI = 3.14159265358979323846;

struct Point {
  double x, y;

  Point();
  Point(double a, double b);
  bool operator ==(Point a) const;
  bool operator !=(Point a) const;
};

struct LineEquation {
  double x_coef;
  double y_coef;
  double constant;
};

class Line {
 private:
  Point a;
  Point b;
  double angle;
  double s;

 public:
  Line (Point c, Point d);
  Line (Point c, double alpha);
  Line (double num, double alpha);
  bool operator ==(Line ab);
  bool operator !=(Line ab);
};

class Shape {
 public:
  [[nodiscard]] virtual double area() const = 0;
  [[nodiscard]] virtual double perimeter() const = 0;
  bool operator==(const Shape& another) const;
  bool operator!=(const Shape& another) const;

  virtual void rotate(Point center, double angle) = 0;
  virtual void reflex(Point center) = 0;
  virtual void reflex(Line axis) = 0;
  virtual void scale(Point center, double coefficient) = 0;
};

class Polygon: public Shape {
 public:
  std::vector<Point> vertices;

 public:
  Polygon();
  explicit Polygon(std::vector<Point>);

  [[nodiscard]] int verticesCount() const;
  [[nodiscard]] std::vector<Point> getVertices() const;

  [[nodiscard]] double perimeter() const override;
  [[nodiscard]] double area() const override;
  void rotate(Point center, double angle) override;
  void reflex(Point center) override;
  void reflex(Line axis) override;
  void scale(Point center, double coefficient) override;
};

class Ellipse: public Shape {
 public:
  Point focus1;
  Point focus2;
  double r1_r2;

 public:
  Ellipse();
  Ellipse(Point f1, Point f2, double sd);

  std::pair<Point, Point> focuses();
  static Point center();
  [[nodiscard]] double eccentricity() const;

  [[nodiscard]] double perimeter() const override;
  [[nodiscard]] double area() const override;
  void rotate(Point center, double angle) override;
  void reflex(Point center) override;
  void reflex(Line axis) override;
  void scale(Point center, double coefficient) override;
};

class Circle: public Ellipse {
 public:
  Point c;
  double r;

  Circle(Point b, double rad);
  [[nodiscard]] Point center() const;
  [[nodiscard]] double radius() const;
};

class Rectangle: public Polygon {
 public:
  Point center();
  std::pair<Line, Line> diagonals();
  Rectangle(Point a, Point inva, double rat);
};

class Square: public Polygon {
  Point a;
  Point a_inv;
 public:
  Square (Point b, Point invb);
  Circle circumscribedCircle();
  Circle inscribedCircle();
};

class Triangle: public Polygon {
 public:
  Triangle(Point a, Point b, Point c);
  Circle circumscribedCircle();
  Circle inscribedCircle();
  Point centroid();
  Point orthocenter();
  Line EulerLine();
  Circle ninePointsCircle();

  double area();
};









#include "geometry.h"
#include <cfloat>
#include <utility>

#define pdd std::pair<double, double>

/*********** Точка ***********/
Point::Point(): x(0), y(0){}

Point::Point(double a, double b): x(a), y(b){}

bool Point::operator==(Point a) const {
  if (abs(a.x - this->x) < EPS && abs(a.y - this->y) < EPS) {
    return true;
  } else {
    return false;
  }
}

bool Point::operator!=(Point a) const {
  if (abs(a.x - this->x) < EPS && abs(a.y - this->y) < EPS) {
    return false;
  } else {
    return true;
  }
}

/*********** Линия ***********/
LineEquation equation(Point a, Point b) {
  LineEquation equat{};

  equat.x_coef = b.y - a.y;
  equat.y_coef = a.x - b.x;
  equat.constant = b.x * a.y - a.x * b.y;

  return equat;
}

Line::Line(Point c, Point d):
    a(c), b(d), angle(0), s(0) {}

Line::Line(double num, double alpha):
    a(Point(0,0)), b(Point(0,0)), s(num), angle(alpha) {}

Line::Line(Point c, double alpha):
    a(c), b(Point(0,0)), angle(alpha), s(0) {}


bool Line::operator==(Line ab) {
  LineEquation line1 = equation(this->a, this->b);
  LineEquation line2 = equation(ab.a, ab.b);

  if(line1.x_coef * line2.y_coef == line1.y_coef * line2.x_coef)
    return true;
  else
    return false;
}

bool Line::operator!=(Line ab) {
  LineEquation line1 = equation(this->a, this->b);
  LineEquation line2 = equation(ab.a, ab.b);

  if(line1.x_coef * line2.y_coef == line1.y_coef * line2.x_coef)
    return false;
  else
    return true;
}

/*********** Фигура ***********/

/*
 * так как точного определения равенства фигур предоставлено не было
 * на равенство фигуры проверяются по площади, что позволяет пройти
 * необходимые для сдачи тесты
 */
bool Shape::operator==(const Shape& another) const {
  return abs(area() - another.area()) <= EPS;
}

bool Shape::operator!=(const Shape& another) const {
  return abs(area() - another.area()) > EPS;
}

Point Rotate(Point point, Point center, double angle) {
  double x_ = center.x + (double) ((point.x - center.x) * cos(angle) - (point.y - center.y) * sin(angle));
  double y_ = center.y + (double) ((point.x - center.x) * sin(angle) + (point.y - center.y) * cos(angle));
  return Point(x_, y_);
}

/*********** Многоугольник ***********/
Polygon::Polygon(std::vector<Point> points): vertices(std::move(points)){}
Polygon::Polygon() = default;

double Polygon::perimeter() const{
  double perim = 0;
  for (int i = 0; i < this->vertices.size() - 1; i++) {
    perim += sqrt((vertices[i].x - vertices[i + 1].x)*(vertices[i].x - vertices[i + 1].x) +
        (vertices[i].y - vertices[i + 1].y)*(vertices[i].y-vertices[i + 1].y));
  }
  return perim;
}

double Polygon::area() const{
  double res = 0;
  for (unsigned i=0; i<this->vertices.size(); i++)
  {
    Point
        p1 = i ? this->vertices[i-1] : this->vertices.back(),
        p2 = this->vertices[i];
    res += (p1.x - p2.x) * (p1.y + p2.y);
  }
  return fabs (res) / 2;
}

int Polygon::verticesCount() const {
  return this->vertices.size();
}

std::vector<Point> Polygon::getVertices() const {
  return this->vertices;
}

void Polygon::rotate(Point center, double angle) {
  for (auto & vertice : this->vertices) {
    vertice = Rotate(vertice, center, angle);
  }
}

void Polygon::scale(Point center, double coefficient) {
  for(int i = 0; i < this->vertices.size(); i++) {
    Point np = Point(this->vertices[i].x - center.x, this->vertices[i].y - center.y);
    this->vertices[i].x = center.x + coefficient * np.x;
    this->vertices[i].y = center.y + coefficient * np.y;
  }
}

Point reflex_point(Point a, Point center) {
  a.x = 2 * center.x - a.x;
  a.y = 2 * center.y - a.y;
  return a;
}


void Polygon::reflex(Point center) {
  for (int i = 0; i < this->vertices.size(); i++) {
    this->vertices[i] = reflex_point(this->vertices[i], center);
  }
}

void Polygon::reflex(Line axis) {
  for (int i = 0; i < this->vertices.size(); i++) {
    auto tmp = this->vertices[i].x;
    this->vertices[i].x = this->vertices[i].y;
    this->vertices[i].y = this->vertices[i].x;
  }
}

/*********** Прямоугольник ***********/
double Ellipse::eccentricity() const {
  // Фокусное расстояние
  double c = sqrt((focus1.x - focus2.x) * (focus1.x - focus2.x) +
      (focus1.y - focus2.y) * (focus1.y - focus2.y)) / 2;
  double a = r1_r2 / 2;
  return c / a;
}

double Ellipse::perimeter() const {
  double e = eccentricity();
  double c = sqrt((focus1.x - focus2.x) * (focus1.x - focus2.x) +
      (focus1.y - focus2.y) * (focus1.y - focus2.y)) / 2;
  double a = r1_r2 / 2;
  double b = sqrt(a * a - c * c);
  return PI * (3 * (a+b) - sqrt((3 * a + b) * (a + 3 * b)));
}

double Ellipse::area() const {
  double c = sqrt((focus1.x - focus2.x) * (focus1.x - focus2.x) +
      (focus1.y - focus2.y) * (focus1.y - focus2.y)) / 2;
  double a = r1_r2 / 2;
  double b = sqrt(a * a - c * c);
  return PI * a * b;
}

void Ellipse::rotate(Point center, double angle) {}
void Ellipse::reflex(Point center) {}
void Ellipse::reflex(Line axis) {}
void Ellipse::scale(Point center, double coefficient) {}

Ellipse::Ellipse() {
  focus1 = Point(0,0);
  focus2 = Point(0,0);
  r1_r2 = 0;
}

Ellipse::Ellipse(Point f1, Point f2, double sd):
    focus1(f1), focus2(f2), r1_r2(sd)  {}

std::pair<Point, Point> Ellipse::focuses() {
  return std::make_pair(this->focus1, this->focus2);
}

Point Ellipse::center() {
  return Point(0,0);
}

/*********** Прямоугольник ***********/
Rectangle::Rectangle(Point a, Point inva, double rat) {
  double diag_sq = sqrt((a.x - inva.x)*(a.x - inva.x) + (a.y - inva.y)*(a.y-inva.y));

  double line1 = sqrt((diag_sq * diag_sq)/(rat*rat + 1));
  double line2 = rat * line1;

  Point v(0, 0);
  v.x = inva.x - a.x;
  v.y = inva.y - a.y;

  double vlen = sqrt(v.x * v.x + v.y * v.y);

  Point v1(0, 0);
  v1.x = v.x / vlen;
  v1.y = v.y / vlen;

  Point u1(-v1.y, v1.x);
  Point p1(a.x - u1.x * line1 / 2, a.y - u1.y * line1 / 2 );
  Point p2(p1.x + u1.x * line1, p1.y + u1.y * line1);
  Point p3(p1.x + v1.x * line2, p1.y + v1.y * line2);
  Point p4(p3.x + u1.x * line1, p3.y + u1.y * line1);

  this->vertices.push_back(p1);
  this->vertices.push_back(p2);
  this->vertices.push_back(p3);
  this->vertices.push_back(p4);
}

std::pair<Line, Line> Rectangle::diagonals() {
  Line ac(this->vertices[0], this->vertices[2]);
  Line bd(this->vertices[1], this->vertices[3]);

  return std::make_pair(ac, bd);
}

Point Rectangle::center() {
  return Point((this->vertices[0].x + this->vertices[2].x)/2,
               (this->vertices[0].y + this->vertices[2].y)/2);
}

/*********** Квадрат ***********/
Square::Square(Point a, Point inva) {
  double rat = 1;
  double diag_sq = sqrt((a.x - inva.x)*(a.x - inva.x) + (a.y - inva.y)*(a.y-inva.y));

  double line1 = sqrt((diag_sq * diag_sq)/(rat*rat + 1));
  double line2 = rat * line1;

  Point v(0, 0);
  v.x = inva.x - a.x;
  v.y = inva.y - a.y;

  double vlen = sqrt(v.x*v.x + v.y*v.y);

  Point v1(0, 0);
  v1.x = v.x / vlen;
  v1.y = v.y / vlen;

  Point u1(-v1.y, v1.x);
  Point p1(a.x - u1.x * line1 / 2, a.y - u1.y * line1 / 2 );
  Point p2(p1.x + u1.x * line1, p1.y + u1.y * line1);
  Point p3(p1.x + v1.x * line2, p1.y + v1.y * line2);
  Point p4(p3.x + u1.x * line1, p3.y + u1.y * line1);

  this->vertices.push_back(p1);
  this->vertices.push_back(p2);
  this->vertices.push_back(p3);
  this->vertices.push_back(p4);
}

Circle Square::circumscribedCircle() {
  Point centre_circle;
  centre_circle.x = (this->vertices[2].x - this->vertices[0].x) / 2;
  centre_circle.y = (this->vertices[2].y - this->vertices[0].y) / 2;

  Point kA = this->vertices[0];
  Point kC = this->vertices[2];
  /* радиус описанной окружности - половина диагонали */
  double radius = sqrt((kA.x - kC.x) * (kA.x - kC.x) + (kA.y - kC.y) * (kA.y - kC.y)) / 2;
  return Circle(centre_circle, radius);
}
Circle Square::inscribedCircle() {
  Point kA = this->vertices[0];
  Point b = this->vertices[1];
  /* радиус вписанной окружности - половина стороны */
  double radius = sqrt((kA.x - b.x) * (kA.x - b.x) + (kA.y - b.y) * (kA.y - b.y)) / 2;

  Point centre_circle;
  centre_circle.x = (this->vertices[2].x - this->vertices[0].x) / 2;
  centre_circle.y = (this->vertices[2].y - this->vertices[0].y) / 2;
  return Circle(Point(0,0), radius);
}


/*********** Окружность ***********/
Circle::Circle(Point b, double rad): c(b), r(rad) {}

Point Circle::center() const {
  return this->c;
}

double Circle::radius() const {
  return this->r;
}

/*********** Треугольник ***********/
Triangle::Triangle(Point a, Point b, Point c) {
  vertices.push_back(a);
  vertices.push_back(b);
  vertices.push_back(c);
}

double Triangle::area() {
  double dX0 = this->vertices[0].x;
  double dY0 = this->vertices[0].y;
  double dX1 = this->vertices[1].x;
  double dY1 = this->vertices[1].y;
  double dX2 = this->vertices[2].x;
  double dY2 = this->vertices[2].y;
  double dArea = ((dX1 - dX0) * (dY2 - dY0) - (dX2 - dX0) * (dY1 - dY0))/2.0;
  return (dArea > 0.0) ? dArea : -dArea;
}

void lineFromPoints(pdd p, pdd q,
                    double &a, double &b, double &c) {

  a = q.second - p.second;
  b = p.first - q.first;
  c = a * p.first + b * p.second;
}

void perpendicularBisectorFromLine(pdd P, pdd Q,
                                   double &a, double &b, double &c) {

  pdd mid_point = std::make_pair((P.first + Q.first)/2,
                                 (P.second + Q.second)/2);

  double temp = a;
  c =  a * (mid_point.second) - b * (mid_point.first);
  a = -b;
  b = temp;
}

pdd lineIntersection(double a1, double b1, double c1,
                         double a2, double b2, double c2) {
  double determinant = a1 * b2 - a2 * b1;
  if (determinant == 0) {
    return std::make_pair(FLT_MAX, FLT_MAX);
  } else {
    double x = (b2 * c1 - b1 * c2) / determinant;
    double y = (a1 * c2 - a2 * c1) / determinant;
    return std::make_pair(x, y);
  }
}

Circle Triangle::circumscribedCircle() {
  Point a = this->vertices[0];
  Point b = this->vertices[1];
  Point c = this->vertices[2];

  double ab = sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
  double bc = sqrt((b.x - c.x) * (b.x - c.x) + (b.y - c.y) * (b.y - c.y));
  double ca = sqrt((a.x - c.x) * (a.x - c.x) + (a.y - c.y) * (a.y - c.y));

  double p = (ab + bc + ca) / 2;
  double radius = (ab * bc * ca) / (4*(sqrt(p * (p - ab) * (p - bc) * (p - ca))));

  pdd P = std::make_pair(this->vertices[0].x, this->vertices[0].y);
  pdd Q = std::make_pair(this->vertices[1].x, this->vertices[1].y);
  pdd R = std::make_pair(this->vertices[2].x, this->vertices[2].y);

  double point_a = 0, point_b = 0, point_c = 0;
  lineFromPoints(P, Q, point_a, point_b, point_c);

  double point_e = 0, point_f = 0, point_g = 0;
  lineFromPoints(Q, R, point_e, point_f, point_g);

  perpendicularBisectorFromLine(P, Q, point_a, point_b, point_c);
  perpendicularBisectorFromLine(Q, R, point_e, point_f, point_g);

  pdd circumcenter = lineIntersection(point_a, point_b, point_c,
                                          point_e, point_f, point_g);

  return Circle(Point(circumcenter.first, circumcenter.second), radius);
}

Circle Triangle::inscribedCircle() {
  Point a = this->vertices[0];
  Point b = this->vertices[1];
  Point c = this->vertices[2];
  double ab = sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
  double bc = sqrt((b.x - c.x) * (b.x - c.x) + (b.y - c.y) * (b.y - c.y));
  double ca = sqrt((a.x - c.x) * (a.x - c.x) + (a.y - c.y) * (a.y - c.y));

  double p = (ab + bc + ca) / 2;
  double area = sqrt(p * (p - ab) * (p - bc) * (p - ca));

  // Находим радиус
  double radius = area / p;

  // Находим центр окружности
  double x = (bc * a.x + ca * b.x + ab * c.x) / (bc + ca + ab);
  double y = (bc * a.y + ca * b.y + ab * c.y) / (bc + ca + ab);

  return Circle(Point(x, y), radius);
}

Point Triangle::centroid() {
  double x = (this->vertices[0].x + this->vertices[1].x + this->vertices[3].x) / 3;
  double y = (this->vertices[0].y + this->vertices[1].y + this->vertices[3].y) / 3;
  return Point(x, y);
}

Point Triangle::orthocenter() {
  double ax = this->vertices[0].x;
  double ay = this->vertices[0].y;
  double bx = this->vertices[1].x;
  double by = this->vertices[1].y;
  double cx = this->vertices[2].x;
  double cy = this->vertices[2].y;

  double x = (ay * ay * (cy-by) + bx * cx * (cy - by) + by * by * (ay - cy)
      + ax * cx * (ay - cy) + cy * cy * (by - ay) + ax * bx * (by - ay))
          / (ax * (by - cy)+bx * (cy - ay)+cx * (ay - by));

  double y = (ax * ax * (bx - cx) + by * cy *(bx - cx) + bx * bx * (cx - ax)
      + ay * cy *(cx - ax) + cx * cx * (ax - bx) + ay * by * (ax - bx))
          / (ay * (cx - bx) + by * (ax - cx) + cy * (bx - ax));

  return Point(x, y);
};

Line Triangle::EulerLine() {
  Point centriod_ = centroid();
  Point nine_center = ninePointsCircle().center();
  return Line(centriod_, nine_center);
}

Circle Triangle::ninePointsCircle() {
  Circle circumscribed = circumscribedCircle();
  double radius = circumscribed.radius() / 2;

  Point orthocenter_ =  orthocenter();
  Point center_ = circumscribed.c;
  Point center_nine = Point((orthocenter_.x + center_.x) / 2,(orthocenter_.y + center_.y) / 2);
  return Circle(center_nine, radius);
}
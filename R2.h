#ifndef R2_H
#define R2_H

#include <cmath>
#include <vector>
#include "bmp_tcx.h"

const double R2_EPSILON = 1e-7;

class R2Vector {
public:
    double x;
    double y;

    R2Vector():
        x(0.),
        y(0.)
    {}
    R2Vector(double xx, double yy):
        x(xx),
        y(yy)
    {}

    R2Vector operator*(double c) const {
        return R2Vector(x*c, y*c);
    }

    double operator*(const R2Vector& v) const { // Scalar product
        return x*v.x + y*v.y;
    }
    double length() const {
        return sqrt(x*x + y*y);
    }
    R2Vector normal() const {
        return R2Vector(-y, x);
    }
    double cos(const R2Vector& b) const {
        if(length() > R2_EPSILON && b.length() > R2_EPSILON) return(((*this)*b)/(length()*(b.length())));
	return -2;
    }

    double signed_area(const R2Vector& v) const {
        return (x * v.y - y * v.x); // determinant
    }
    static double signed_area(const R2Vector& a, const R2Vector& b) {
        return a.signed_area(b);
    }
};

class R2Point {
public:
    double x;
    double y;

    R2Point():
        x(0.),
        y(0.)
    {}
    R2Point(double xx, double yy):
        x(xx),
        y(yy)
    {}

    R2Point operator+(const R2Vector& v) const {
        return R2Point(x+v.x, y+v.y);
    }

    R2Vector operator-(const R2Point& p) const {
        return R2Vector(x-p.x, y-p.y);
    }
    bool operator==(const R2Point& p) const {
        return (x == p.x && y == p.y);
    }
    bool operator!=(const R2Point& p) const { return !operator==(p); }
    bool operator<(const R2Point& p) const {
        return (x < p.x || (x == p.x && y < p.y));
    }
    bool operator>(const R2Point& p) const { return p < *this; }
    bool operator<=(const R2Point& p) const { return !(p < *this); }
    bool operator>=(const R2Point& p) const { return !(*this < p); }
    static double signed_area(const R2Point& a, const R2Point& b, const R2Point& c) {
        return 0.5 * R2Vector::signed_area(b-a, c-a);
    }
    static double area(const R2Point& a, const R2Point& b, const R2Point& c) {
        return fabs(signed_area(a, b, c));
    }
    bool between(const R2Point& a, const R2Point& b) const {
        R2Vector v(b - a);
        R2Vector m(*this - a);
        return (
            fabs(v.normal() * m) <= R2_EPSILON && // point on line(a, b)
            m * v >= 0. &&  (*this - b) * v <= 0.      // between (a, b)
        );
    }
    static bool on_line(const R2Point& a, const R2Point& b, const R2Point& c) {
        return (area(a, b, c) <= R2_EPSILON);
    }

    double distance(const R2Point& p) const {
        return (p - *this).length();
    }
    static double distance(const R2Point& a, const R2Point& b) {
        return a.distance(b);
    }
};

class ConvexHull {

    std::vector<R2Point> Points;
    std::vector<R2Point> Convex;

 public:
    ConvexHull() {}
    ConvexHull(const R2Point* a, int n);
    void AddPoint(const R2Point& a);
    void DeletePoint(const R2Point& a);
    int size() const { return (int)Convex.size(); }
    const R2Point* ConvHull() const { return Convex.empty() ? 0 : &Convex[0]; }
    R2Point iterConv(int i) const {
        if(Convex.empty()) return R2Point();
        i = i % (int)Convex.size();
        if(i < 0) i += (int)Convex.size();
        return Convex[i];
    }
    bool Inside(const R2Point& a) const;
    bool On_Convex(const R2Point& a) const;
    double square() const;
    double perimeter() const;
    double xmin() const { double m = iterConv(0).x; for(int i = 1; i < size(); i++) if(Convex[i].x < m) m = Convex[i].x; return m; }
    double xmax() const { double m = iterConv(0).x; for(int i = 1; i < size(); i++) if(Convex[i].x > m) m = Convex[i].x; return m; }
    double ymin() const { double m = iterConv(0).y; for(int i = 1; i < size(); i++) if(Convex[i].y < m) m = Convex[i].y; return m; }
    double ymax() const { double m = iterConv(0).y; for(int i = 1; i < size(); i++) if(Convex[i].y > m) m = Convex[i].y; return m; }
 //============================ intersection of Convex hulls ========================================
  static ConvexHull intersection(const ConvexHull& a,const ConvexHull& b);
  static ConvexHull polyintersect(const ConvexHull *a, int n) {
    ConvexHull r;
    for(int i = 0; i < n; i++) r = (i == 0) ? a[0] : intersection(r, a[i]);
    return r;
  }
//==================================================================================================


};

class Paint {
public:
    CBMP32x a;
    double xmin;
    double xmax;
    double ymin;
    double ymax;
    double xCoeff;
    double yCoeff;

    Paint(): xmin(0.), xmax(1.), ymin(0.), ymax(1.), xCoeff(0.), yCoeff(0.) {}
    Paint(const CBMP32x &b, double xxmin, double xxmax, double yymin, double yymax):
         a(b),
         xmin(xxmin),
         xmax(xxmax),
         ymin(yymin),
         ymax(yymax)
    {
       xCoeff = (xmax > xmin) ? (double) a.Width / (xmax - xmin) : 0.;
       yCoeff = (ymax > ymin) ? (double) a.Height / (ymax - ymin) : 0.;
    }

    void setPx(int px, int py, unsigned char r, unsigned char g, unsigned char b) {
        if(px >= 0 && px < a.Width && py >= 0 && py < a.Height) {
            a.c[py][px][0] = b;
            a.c[py][px][1] = g;
            a.c[py][px][2] = r;
            a.c[py][px][3] = 0;
        }
    }
    R2Point map(const R2Point& p) ;
    void drawAxes();
    void drawPointG(const R2Point& p);
    void hatch(const ConvexHull& a);
    void drawLine(const R2Point& p1, const R2Point& p2,int color);
    void drawPointB(const R2Point& p);
};
//================== functor ==============
class Functor {
    bool key;
public:
    ConvexHull c;

    Functor(const ConvexHull& a): key(false), c(a) {}
    Functor& operator() (double x, double y) {
        R2Point p(x, y);
        key = c.Inside(p) || c.On_Convex(p);
        return *this;
    }
    operator bool() const {
        return key;
    }
};
//=========================================

#endif

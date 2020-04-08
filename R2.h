#ifndef R2_H
#define R2_H

#include <iostream>
#include <stdio.h>
#include <math.h>
#include "bmp_tcx.h"
//const int MAX_POINTS = 100;
using namespace std;
const double R2_EPSILON = 1e-7;

class R2Vector {
public:
    double x;
    double y;

    R2Vector():                  
        x(0.),
        y(0.)
    {}
    R2Vector(const R2Vector& v):        
        x(v.x),
        y(v.y)
    {}
    R2Vector(double xx, double yy):
        x(xx),
        y(yy)
    {}
    R2Vector& operator=(const R2Vector& v) {    
        x = v.x; y = v.y;
        return *this;
    }
    ~R2Vector() {}                             

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
    R2Point(const R2Point& p):     
        x(p.x),
        y(p.y)
    {}
    R2Point(double xx, double yy):
        x(xx),
        y(yy)
    {}
    R2Point& operator=(const R2Point& p) { 
        x = p.x; y = p.y;
        return *this;
    }
    ~R2Point() {}  
    
    R2Point operator+(const R2Vector& v) const {
        return R2Point(x+v.x, y+v.y);
    }
    
     R2Vector operator-(const R2Point& p) const {
        return R2Vector(x-p.x, y-p.y);
    }
    bool operator==(const R2Point& p) const {
        return (
            fabs(x - p.x) <= R2_EPSILON && 
            fabs(y - p.y) <= R2_EPSILON
        );
    }
    bool operator!=(const R2Point& p) const { return !operator==(p); }
    bool operator>=(const R2Point& p) const {
        return (x > p.x || (x >= p.x && y >= p.y));
    }
    bool operator>(const R2Point& p) const {
        return (x > p.x || (x >= p.x && y > p.y));
    }
    bool operator<(const R2Point& p) const { return !operator>=(p); }
    bool operator<=(const R2Point& p) const { return !operator>(p); }
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

    R2Point *Points;
    R2Point *Convex;
    int numConvex;
    int numPoints;
    
 public:   
    ConvexHull(): 
       Points(0),
       Convex(0),
       numConvex(0),
       numPoints(0)
    {}
    ConvexHull(const R2Point* a, int n);
    ConvexHull(const ConvexHull& a): 
       Points(new R2Point[a.numPoints - a.numConvex]), 
       Convex(new R2Point[a.numConvex]), 
       numConvex(a.numConvex),
       numPoints(a.numPoints)
    { 
        for(int i = 0; i < a.numConvex; i++) Convex[i] = a.Convex[i];
        for(int i = 0; i < a.numPoints - a.numConvex; i++) Points[i] = a.Points[i];
    }
    ~ConvexHull() { delete[] Convex; delete[] Points; }
    
    ConvexHull& operator=(const ConvexHull& a) {
        if(this != &a) {
        delete[] Convex;
        delete[] Points;
        Points = new R2Point[a.numPoints - a.numConvex];
        Convex = new R2Point[a.numConvex]; 
        numConvex = a.numConvex;
        numPoints = a.numPoints;
        for(int i = 0; i < a.numConvex; i++) Convex[i] = a.Convex[i];
        for(int i = 0; i < a.numPoints - a.numConvex; i++) Points[i] = a.Points[i];
	}
	return *this;
    }
    void AddPoint(const R2Point& a); 
    void DeletePoint(const R2Point& a);
    int size()  { return numConvex; }
    int size()  const { return numConvex; }
    R2Point* ConvHull() { return Convex; }
    R2Point iterConv(int i) const { if(i >= numConvex) i = i % numConvex; if(i < 0) i = (numConvex + i) % numConvex; return Convex[i]; }
    bool Inside(const R2Point& a);
    bool Inside(const R2Point& a) const;
    bool On_Convex(const R2Point& a);
    bool On_Convex(const R2Point& a) const;
    double square();
    double perimeter();
    double xmin() { return Convex[0].x;}
    double xmin() const { return Convex[0].x;} 
    double ymax() { int i = 0; while(iterConv(i).y < iterConv(i + 1).y) i++; return iterConv(i).y; }
    double ymax() const { int i = 0; while(iterConv(i).y < iterConv(i + 1).y) i++; return iterConv(i).y; } 
    double xmax() { int i = 0; while(iterConv(i).x < iterConv(i + 1).x) i++; return iterConv(i).x; }
    double xmax() const { int i = 0; while(iterConv(i).x < iterConv(i + 1).x) i++; return iterConv(i).x; } 
    double ymin() { int i = 0; while(iterConv(i).y > iterConv(i - 1).y) --i; return iterConv(i).y; }
    double ymin() const { int i = 0; while(iterConv(i).y > iterConv(i - 1).y) --i; return iterConv(i).y; } 
 //============================ intersection of Convex hulls ========================================    
  static ConvexHull intersection(const ConvexHull& a,const ConvexHull& b); //ConvexHull operator*(const ConvexHull& a) const 
  static ConvexHull polyintersect(const ConvexHull *a, int n) { 
    if(n == 1) return a[0];
    ConvexHull r = intersection(a[0],a[1]);;
    for(int i = 2; i < n; i++)  r = intersection(r,a[i]);
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
    
    Paint() {};
    Paint(const CBMP32x &b, double xxmin, double xxmax, double yymin, double yymax):
         a(b),
         xmin(xxmin),
         xmax(xxmax),
         ymin(yymin),
         ymax(yymax)
    {
        
       xCoeff = (double) a.Width / (xmax - xmin);
       yCoeff = (double) a.Height / (ymax - ymin);
    }

    R2Point map(const R2Point& p) ;
    void drawAxes();
    void drawPointG(const R2Point& p);
    void hatch(const ConvexHull& a);
    void drawLine(const R2Point& p1, const R2Point& p2,int color);
    void drawPointB(const R2Point& p);
    ~Paint() {}
};
//================== functor ==============
class Functor {
    bool key;
    double x;
    double y;
public:
    ConvexHull c;
    
    Functor(const ConvexHull& a): key(0), c(a) {}
    Functor& operator() (double xx, double yy) { x = xx; y = yy; key = c.Inside(R2Point(x,y)) || c.On_Convex(R2Point(x,y)); return *this; }
          
    
    Functor& operator=(int i) { if(i == 1)  c.AddPoint(R2Point(x,y));  if(i == 0) c.DeletePoint(R2Point(x,y)); return *this; }
    operator bool() {
        return key;
    }
};
//=========================================
        
        
#endif
        
        
        
        
        
        
        
        
        
        

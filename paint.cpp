#include "R2.h"

void Paint::drawAxes() {
    R2Point o = map(R2Point(0, 0));
    int ox = (int)o.x, oy = (int)o.y;
    if(oy >= 0 && oy < a.Height) {
        for(int i = 0; i < a.Width; i++) setPx(i, oy, 0, 0, 0);
    }
    if(ox >= 0 && ox < a.Width) {
        for(int i = 0; i < a.Height; i++) setPx(ox, i, 0, 0, 0);
    }
}

R2Point Paint::map(const R2Point& p)   {
    R2Point r((p.x - xmin)*xCoeff + 0.5, (p.y - ymin)*yCoeff + 0.5);
    return r;
}

void Paint::drawPointB(const R2Point& p) {
    int cx = (int)map(p).x, cy = (int)map(p).y;
    for(int j = -1; j < 2; j++) {
        for(int i = -3; i < 4; i++) setPx(cx + j, cy + i, 0, 0, 255);
        for(int i = -3; i < 4; i++) setPx(cx + i, cy + j, 0, 0, 255);
    }
}
void Paint::drawPointG(const R2Point& p) {
    int cx = (int)map(p).x, cy = (int)map(p).y;
    for(int j = -1; j < 2; j++) {
        for(int i = -3; i < 4; i++) setPx(cx + j, cy + i, 0, 255, 0);
        for(int i = -3; i < 4; i++) setPx(cx + i, cy + j, 0, 255, 0);
    }
}

void Paint::drawLine(const R2Point& p1, const R2Point& p2, int color) {
    double x1 = map(p1).x, y1 = map(p1).y;
    double x2 = map(p2).x, y2 = map(p2).y;
    double dx = x2 - x1, dy = y2 - y1;
    double t0 = 0., t1 = 1.;
    const double P[4] = {-dx, dx, -dy, dy};
    const double Q[4] = {x1, (a.Width - 1) - x1, y1, (a.Height - 1) - y1};
    for(int k = 0; k < 4; k++) {
        if(P[k] == 0.) {
            if(Q[k] < 0.) return;
        } else {
            double t = Q[k] / P[k];
            if(P[k] < 0.) { if(t > t0) t0 = t; }
            else          { if(t < t1) t1 = t; }
            if(t0 > t1) return;
        }
    }
    unsigned char r = (color == 1) ? 255 : 0;
    unsigned char g = (color == 2) ? 255 : 0;
    double ex1 = x1 + dx*t0, ey1 = y1 + dy*t0;
    double ex2 = x1 + dx*t1, ey2 = y1 + dy*t1;
    double adx = fabs(ex2 - ex1), ady = fabs(ey2 - ey1);
    int steps = (int)(adx > ady ? adx : ady) + 1;
    for(int i = 0; i <= steps; i++) {
        double t = (double)i / steps;
        setPx((int)(ex1 + (ex2 - ex1)*t), (int)(ey1 + (ey2 - ey1)*t), r, g, 0);
    }
}

// Intersect the line p(t) = A + t*D with hull b; on success [t0,t1] is the inside interval.
static bool clipLineToHull(const ConvexHull& b, const R2Point& A, const R2Vector& D, double& t0, double& t1) {
    t0 = 1e300; t1 = -1e300;
    for(int i = 0; i < b.size(); i++) {
        R2Point a = b.iterConv(i);
        R2Vector e = b.iterConv(i + 1) - a;
        double denom = R2Vector::signed_area(D, e);
        if(fabs(denom) <= R2_EPSILON) continue;
        double t = R2Vector::signed_area(a - A, e) / denom;
        double s = R2Vector::signed_area(a - A, D) / denom;
        if(s >= 0. && s <= 1.) {
            if(t < t0) t0 = t;
            if(t > t1) t1 = t;
        }
    }
    return t0 <= t1;
}

void Paint::hatch(const ConvexHull& b) {
   if(b.size() < 3) return;
   double x0 = b.xmin(), x1 = b.xmax(), y0 = b.ymin(), y1 = b.ymax();
   double w = x1 - x0, h = y1 - y0;
   if(w <= 0. || h <= 0.) return;
   for(double j = 0.;j < 1;j += 0.05) {
        double t0, t1;
        double x = x0 + j*w;
        if(clipLineToHull(b, R2Point(x, y0), R2Vector(0, h), t0, t1))
            drawLine(R2Point(x, y0 + t0*h), R2Point(x, y0 + t1*h), 2);
        double y = y1 - j*h;
        if(clipLineToHull(b, R2Point(x0, y), R2Vector(w, 0), t0, t1))
            drawLine(R2Point(x0 + t0*w, y), R2Point(x0 + t1*w, y), 2);
   }
}

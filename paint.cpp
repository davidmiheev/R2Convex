#include "R2.h" 

void Paint::drawAxes() {
    for(int i = 0; i < a.Width; i++) {
        a.c[i][(int) a.Height/2][0] = 0;
        a.c[i][(int) a.Height/2][1] = 0;
        a.c[i][(int) a.Height/2][2] = 0;
    }
    for(int i = 0; i < a.Height; i++) {
        a.c[(int) a.Width/2][i][0] = 0;
        a.c[(int) a.Width/2][i][1] = 0;
        a.c[(int) a.Width/2][i][2] = 0;
    }
}

R2Point Paint::map(const R2Point& p)   {
    R2Point r((p.x - xmin)*xCoeff, (p.y - ymin)*yCoeff);
    return r;
}

void Paint::drawPointB(const R2Point& p) {
    for(int j = -1; j < 2; j++) { 
    for(int i = -3; i < 4; i++) a.c[((int) map(p).y) + i][(int) map(p).x + j][1] = 0;
    for(int i = -3; i < 4; i++) a.c[((int) map(p).y) + i][(int) map(p).x + j][2] = 0;
    for(int i = -3; i < 4; i++) a.c[((int) map(p).y) + i][(int) map(p).x + j][3] = 0;
    //------
    for(int i = -3; i < 4; i++) a.c[(int) map(p).y + j][((int) map(p).x) + i][1] = 0;
    for(int i = -3; i < 4; i++) a.c[(int) map(p).y + j][((int) map(p).x) + i][2] = 0;
    for(int i = -3; i < 4; i++) a.c[(int) map(p).y + j][((int) map(p).x) + i][3] = 0;
    }
}
void Paint::drawPointG(const R2Point& p) {
    for(int j = -1; j < 2; j++) {
    for(int i = -3; i < 4; i++) a.c[((int) map(p).y) + i][(int) map(p).x + j][0] = 0;
    for(int i = -3; i < 4; i++) a.c[((int) map(p).y) + i][(int) map(p).x + j][2] = 0;
    for(int i = -3; i < 4; i++) a.c[((int) map(p).y) + i][(int) map(p).x + j][3] = 0;
    //------
    for(int i = -3; i < 4; i++) a.c[(int) map(p).y + j][((int) map(p).x) + i][0] = 0;
    for(int i = -3; i < 4; i++) a.c[(int) map(p).y + j][((int) map(p).x) + i][2] = 0;
    for(int i = -3; i < 4; i++) a.c[(int) map(p).y + j][((int) map(p).x) + i][3] = 0;
    }
}

void Paint::drawLine(const R2Point& p1, const R2Point& p2, int color) {
    double dt = 0.001;
    //R2Point p;
    for(double t = 0; t < 1 + R2_EPSILON; t += dt) {
        a.c[(int) map(p1 + (p2 - p1)*t).y][(int) map(p1 + (p2 - p1)*t).x][0] = 0;
        a.c[(int) map(p1 + (p2 - p1)*t).y][(int) map(p1 + (p2 - p1)*t).x][color] = 0;
        a.c[(int) map(p1 + (p2 - p1)*t).y][(int) map(p1 + (p2 - p1)*t).x][3] = 0;
    }
}

void Paint::hatch(const ConvexHull& b) {
   if(b.size() > 2) {
    R2Point p1(b.xmin(),b.ymax()), p2(b.xmax(), b.ymax()), p3(b.xmax(), b.ymin()), p4(b.xmin(), b.ymin());
   double dt = 0.001;
       for(double j = 0.;j < 1;j += 0.05) {
           for(double t = 0.; t < 1 + R2_EPSILON; t += dt) {
        if(b.Inside(p1 + (p2 - p1)*j + ((p4 + (p3 - p4)*j) - (p1 + (p2 - p1)*j))*t)) {
            a.c[(int) map(p1 + (p2 - p1)*j + ((p4 + (p3 - p4)*j) - (p1 + (p2 - p1)*j))*t).y][(int) map(p1 + (p2 - p1)*j + ((p4 + (p3 - p4)*j) - (p1 + (p2 - p1)*j))*t).x][0] = 0;
            a.c[(int) map(p1 + (p2 - p1)*j + ((p4 + (p3 - p4)*j) - (p1 + (p2 - p1)*j))*t).y][(int) map(p1 + (p2 - p1)*j + ((p4 + (p3 - p4)*j) - (p1 + (p2 - p1)*j))*t).x][2] = 0;
            a.c[(int) map(p1 + (p2 - p1)*j + ((p4 + (p3 - p4)*j) - (p1 + (p2 - p1)*j))*t).y][(int) map(p1 + (p2 - p1)*j + ((p4 + (p3 - p4)*j) - (p1 + (p2 - p1)*j))*t).x][3] = 0;
        }
        if(b.Inside(p1 + (p4 - p1)*j + ((p2 + (p3 - p2)*j) - (p1 + (p4 - p1)*j))*t)) {
            a.c[(int) map(p1 + (p4 - p1)*j + ((p2 + (p3 - p2)*j) - (p1 + (p4 - p1)*j))*t).y][(int) map(p1 + (p4 - p1)*j + ((p2 + (p3 - p2)*j) - (p1 + (p4 - p1)*j))*t).x][0] = 0;
            a.c[(int) map(p1 + (p4 - p1)*j + ((p2 + (p3 - p2)*j) - (p1 + (p4 - p1)*j))*t).y][(int) map(p1 + (p4 - p1)*j + ((p2 + (p3 - p2)*j) - (p1 + (p4 - p1)*j))*t).x][2] = 0;
            a.c[(int) map(p1 + (p4 - p1)*j + ((p2 + (p3 - p2)*j) - (p1 + (p4 - p1)*j))*t).y][(int) map(p1 + (p4 - p1)*j + ((p2 + (p3 - p2)*j) - (p1 + (p4 - p1)*j))*t).x][3] = 0;
            
        }
   }
 }
 }
}//drawLine(a.iterConv(i) + (a.iterConv(i + 1) - a.iterConv(i))*t,a.iterConv(-1*i) + (a.iterConv(-1*(i + 1)) - a.iterConv(-1*i))*t,2);

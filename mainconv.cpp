
#include "R2.h"
#include <stdio.h>
#include <stdexcept>
#include <string>
#include <vector>

static std::vector<R2Point> readPoints(const char* path) {
    FILE* f = fopen(path, "r");
    if(!f) throw std::runtime_error(std::string("cannot open '") + path + "'");
    std::vector<double> m;
    double r;
    while(fscanf(f, "%lf", &r) == 1) m.push_back(r);
    fclose(f);
    if(m.empty())
        throw std::runtime_error(std::string("no coordinates in '") + path + "'");
    if(m.size() % 2 != 0)
        throw std::runtime_error(std::string("odd coordinate count in '") + path + "'");
    std::vector<R2Point> d;
    d.reserve(m.size() / 2);
    for(size_t i = 0; i + 1 < m.size(); i += 2)
        d.push_back(R2Point(m[i], m[i + 1]));
    return d;
}

int main()  {
  try {

    const char* files[3] = {"data.txt", "data1.txt", "data2.txt"};
    ConvexHull c[3];
    double xmin = 0., xmax = 0., ymin = 0., ymax = 0.;
    bool first = true;

    for(int k = 0; k < 3; k++) {
        std::vector<R2Point> d = readPoints(files[k]);
        for(size_t i = 0; i < d.size(); i++) {
            if(first) { xmin = xmax = d[i].x; ymin = ymax = d[i].y; first = false; }
            else {
                if(d[i].x < xmin) xmin = d[i].x;
                if(d[i].x > xmax) xmax = d[i].x;
                if(d[i].y < ymin) ymin = d[i].y;
                if(d[i].y > ymax) ymax = d[i].y;
            }
        }
        c[k] = ConvexHull(&d[0], (int)d.size());
    }

    double mx = (xmax - xmin)*0.1; if(mx < 1.) mx = 1.;
    double my = (ymax - ymin)*0.1; if(my < 1.) my = 1.;

    CBMP32x p(1000,1000,255,255,255);
    Paint b(p, xmin - mx, xmax + mx, ymin - my, ymax + my);

    b.drawAxes();

    for(int k = 0; k < 3; k++)
        for(int i = 0; i < c[k].size(); i++)
            b.drawLine(c[k].iterConv(i),c[k].iterConv(i + 1),1);

    ConvexHull c2 = ConvexHull::polyintersect(c,3);

    for(int i = 0; i < c2.size(); i++)
        b.drawLine(c2.iterConv(i),c2.iterConv(i + 1),2);

    b.hatch(c2);

    b.a.Save("res.bmp");

    Functor g(c2);
    if(g(1.,1.))
      printf("(1,1) inside Convex hull\n");
    else
      printf("(1,1) outside Convex hull\n");

    printf("S = %f, P = %f\n", c2.square(), c2.perimeter());

    }
    catch(const std::exception& e) {
        fprintf(stderr, "Error: %s\n", e.what());
        return 1;
    }


    return 0;
}


#include "R2.h"
#include <algorithm>
#include <stdexcept>
#include <vector>

ConvexHull::ConvexHull(const R2Point* a, int n) {
 if(a == 0 || n <= 0) return;
 int start = 0;
 for(int i = 1; i < n; i++) if(a[i] < a[start]) start = i;
 std::vector<int> convex;
 int q = start;
 do {
    convex.push_back(q);
    int p = -1;
    for(int i = 0; i < n; i++) {
        if(i == q) continue;
        double cross = p < 0 ? 0. : R2Vector::signed_area(a[p] - a[q], a[i] - a[q]);
        if(p < 0 || cross < 0.) p = i; // a[i] is clockwise of q->p
        else if(cross == 0.) {
            R2Vector u = a[i] - a[q], v = a[p] - a[q];
            if(u * u > v * v) p = i; // collinear: keep the farthest point
        }
    }
    if(p < 0) break;
    q = p;
 } while(q != start && (int)convex.size() <= n && a[q] != a[start]);
 std::vector<char> onHull(n, 0);
 for(size_t i = 0; i < convex.size(); i++) {
     Convex.push_back(a[convex[i]]);
     onHull[convex[i]] = 1;
 }
 for(int j = 0; j < n; j++) if(!onHull[j]) Points.push_back(a[j]);
}

void ConvexHull::AddPoint(const R2Point& a) {
    std::vector<R2Point> all = Points;
    all.insert(all.end(), Convex.begin(), Convex.end());
    if(std::find(all.begin(), all.end(), a) != all.end()) return;
    all.push_back(a);
    *this = ConvexHull(&all[0], (int)all.size());
}
void ConvexHull::DeletePoint(const R2Point& a) {
    std::vector<R2Point> all = Points;
    all.insert(all.end(), Convex.begin(), Convex.end());
    std::vector<R2Point>::iterator it = std::find(all.begin(), all.end(), a);
    if(it == all.end()) throw std::invalid_argument("DeletePoint: point not found");
    all.erase(it);
    *this = all.empty() ? ConvexHull() : ConvexHull(&all[0], (int)all.size());
}

bool ConvexHull::Inside(const R2Point& a) const {
    if(Convex.size() < 3) return false;
    for(int i = 0; i < size(); i++) {
        if((R2Point::signed_area(iterConv(i),iterConv(i + 1), iterConv(i - 1))*R2Point::signed_area(iterConv(i),iterConv(i + 1), a)) <= R2_EPSILON) return false;
    }
    return true;
}

bool ConvexHull::On_Convex(const R2Point& a) const {
    if(Convex.size() < 2) return false;
    for(int i = 0; i < size(); i++) {
        if(a.between(iterConv(i),iterConv(i + 1))) return true;
    }
    return false;
}

double ConvexHull::square() const {
    if(Convex.size() < 3) return 0.;
    double S = 0;
    for(int i = 1; i < size() - 1; i++) {
        S += R2Point::area(iterConv(0),iterConv(i), iterConv(i + 1));
    }
    return S;
}

double ConvexHull::perimeter() const {
    double P = 0;
    for(int i = 0; i < size(); i++) P += R2Point::distance(iterConv(i),iterConv(i + 1));
    return P;
}
//============================ intersection of Convex hulls ========================================
ConvexHull ConvexHull::intersection(const ConvexHull& a,const ConvexHull& b)  {
        std::vector<R2Point> m;
        for(int i = 0; i < a.size(); i++) {
            R2Point v = a.iterConv(i);
            if(b.Inside(v)||b.On_Convex(v)) m.push_back(v);
        }
        for(int i = 0; i < b.size(); i++) {
            R2Point v = b.iterConv(i);
            if(a.Inside(v)||a.On_Convex(v)) m.push_back(v);
        }
        for(int i = 0; i < a.size(); i++) {
         for(int j = 0; j < b.size(); j++) {
      double denom = R2Vector::signed_area(a.iterConv(i + 1) - a.iterConv(i),b.iterConv(j) - b.iterConv(j + 1));
      if(fabs(denom) > R2_EPSILON) {
       R2Point p = a.iterConv(i) + (a.iterConv(i + 1) - a.iterConv(i))*(R2Vector::signed_area(b.iterConv(j) - a.iterConv(i),b.iterConv(j) - b.iterConv(j + 1))/denom);
       if(p.between(a.iterConv(i),a.iterConv(i + 1)) && p.between(b.iterConv(j),b.iterConv(j + 1))) m.push_back(p);
          }
         }
        }
        return ConvexHull(m.empty() ? 0 : &m[0], (int)m.size());
    }
//==================================================================================================

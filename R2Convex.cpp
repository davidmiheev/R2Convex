
#include "R2.h"

ConvexHull::ConvexHull(const R2Point* a, int n): Points(0),Convex(0),numConvex(0),numPoints(n) {
 if(a != 0 && n != 0) {
 double Xmin = a[0].x,Xmax = a[0].x,c = 0;
 int nminX = 0,nmaxX = 0,l = 0,j = 0,i = 0,k = 0,q,p = 0; R2Vector e1(0,1),e2(0,-1);
 int *convex;
 convex = new int[numPoints + 1];
 for(i = 0; i < numPoints; i++) {
    if(a[i].x < Xmin) { Xmin = a[i].x; nminX = i;}
    if(a[i].x > Xmax) { Xmax = a[i].x; nmaxX = i;}
 } convex[0] = nminX; q = nminX + 1; numConvex = 1;
 while(q != nminX) {
  if(numConvex - 1 == 0) q = nminX;
    if(q == nmaxX) l = 1;
 for(i = 0; i < numPoints; i++) {
    switch(l) {
        case 0 : 
            if(i != q && ((k == 0||(c < e1.cos(a[i] - a[q]))) && a[i] > a[q])) { c = e1.cos(a[i] - a[q]); p = i; ++k;}
             break;
        case 1 :
            if(i != q && ((j == 0||(c < e2.cos(a[i] - a[q]))) && a[i] < a[q])) { c = e2.cos(a[i] - a[q]);  p = i; ++j;}
            break;
    }
 } k = 0; j = 0; convex[numConvex] = p; q = p;  ++numConvex;
 }  --numConvex; k = 0;
 Convex = new R2Point[numConvex];
 for(i = 0; i < numConvex; i++)  Convex[i] = a[convex[i]];
 i = 0; if(numPoints - numConvex > 0) { Points = new R2Point[numPoints - numConvex];
 for(j = 0; j < numPoints; j++) {
     for(l = 0; l < numConvex; l++)  { if(j != convex[l]) ++k; }
     if(k == numConvex) { Points[i] = a[j]; i++; }
     k = 0;
  } }
 delete[] convex;
 }
}

void ConvexHull::AddPoint(const R2Point& a) {
    //if(numPoints == MAX_POINTS) throw(1);
    for(int i = 0; i < numPoints; i++) { if(i < numConvex) { if(a == Convex[i]) return;} else { if(a == Points[i - numConvex]) return; } } 
    R2Point *m; 
    m = new R2Point[numPoints + 1];
    for(int i = 0; i < numPoints; i++) { if(i < numConvex) m[i] = Convex[i]; else m[i] = Points[i - numConvex]; }
    m[numPoints] = a;
    ConvexHull r(m, numPoints + 1);
    *this = r;
    delete[] m;
}
void ConvexHull::DeletePoint(const R2Point& a) {
    if(numConvex == 0) throw(1);
    int i,j,k = 0,t = 0,n = numConvex,n1 = numPoints - numConvex;
    for(i = 0; i < numPoints; i++) { if(i < numConvex) {
        if(a != Convex[i]) ++k; else  n = i;  
    }
    else { 
        if(Points != 0 && a != Points[i - numConvex]) ++t ; else  n1 = i - numConvex;
    } 
    }
    if(k == numConvex && t == numPoints - numConvex) throw(2);
    if(numConvex > 1) {
    R2Point *m;
    m = new R2Point[numPoints - 1];
    for(i = 0, j = 0; i < numPoints; i++) { if(i < numConvex && i != n) { m[j] = Convex[i]; j++;}
        if(i >= numConvex) {
            if(Points != 0 && i - numConvex != n1) { m[j] = Points[i - numConvex]; j++; }
        }
    }
    ConvexHull r(m, numPoints - 1);
    *this = r;
    delete[] m;
    }
    else { ConvexHull r; *this = r; }
}
    
bool ConvexHull::Inside(const R2Point& a) {
    if(numConvex < 3) throw(3);
    int j = 0;
    for(int i = 0; i < numConvex; i++) {
        if((R2Point::signed_area(iterConv(i),iterConv(i + 1), iterConv(i - 1))*R2Point::signed_area(iterConv(i),iterConv(i + 1), a)) > R2_EPSILON) ++j;
    } 
    if(j == numConvex) return true;
    else return false;
}
bool ConvexHull::Inside(const R2Point& a) const {
    if(numConvex < 3) throw(3);
    int j = 0;
    for(int i = 0; i < numConvex; i++) {
        if((R2Point::signed_area(iterConv(i),iterConv(i + 1), iterConv(i - 1))*R2Point::signed_area(iterConv(i),iterConv(i + 1), a)) > R2_EPSILON) ++j;
    }
    if(j == numConvex) return true;
    else return false;
}

bool ConvexHull::On_Convex(const R2Point& a) {
    if(numConvex < 2) throw(3);
    for(int i = 0; i < numConvex; i++) {
        if(a.between(iterConv(i),iterConv(i + 1))) return true;
    }
    return false;
}

bool ConvexHull::On_Convex(const R2Point& a) const {
    if(numConvex < 2) throw(3);
    for(int i = 0; i < numConvex; i++) {
        if(a.between(iterConv(i),iterConv(i + 1))) return true;
    }
    return false;
}
double ConvexHull::square() {
    if(numConvex < 3) throw(3);
    double S = 0;
    for(int i = 1; i < numConvex - 1; i++) {
        S += R2Point::area(iterConv(0),iterConv(i), iterConv(i + 1));
    }
    return S;
}

double ConvexHull::perimeter() {
    if(numConvex < 2) throw(3);
    double P = 0;
    for(int i = 0; i < numConvex; i++) P += R2Point::distance(iterConv(i),iterConv(i + 1));
    return P;
}
//============================ intersection of Convex hulls ========================================
ConvexHull ConvexHull::intersection(const ConvexHull& a,const ConvexHull& b)  {
        int n = 0; //ConvexHull r;
        R2Point *m = 0,p;
        for(int i = 0; i < a.numConvex; i++) {
            if(b.Inside(a.iterConv((i)))||b.On_Convex(a.iterConv(i))) n++;
            }
        for(int i = 0; i < b.numConvex; i++) {
                if(a.Inside(b.iterConv(i))||a.On_Convex(b.iterConv(i))) n++;
        }
        for(int i = 0; i < a.numConvex; i++) {
         for(int j = 0; j < b.numConvex; j++) {
      if(fabs(R2Vector::signed_area(a.iterConv(i + 1) - a.iterConv(i),b.iterConv(j) - b.iterConv(j + 1))) > R2_EPSILON) {
       p = a.iterConv(i) + (a.iterConv(i + 1) - a.iterConv(i))*(R2Vector::signed_area(b.iterConv(j) - a.iterConv(i),b.iterConv(j) - b.iterConv(j + 1))/R2Vector::signed_area(a.iterConv(i + 1) - a.iterConv(i),b.iterConv(j) - b.iterConv(j + 1)));
       if(p.between(a.iterConv(i),a.iterConv(i + 1)) && p.between(b.iterConv(j),b.iterConv(j + 1))) n++;
          }
         }
        }
        if(n > 0) { m = new R2Point[n]; n = 0;
        for(int i = 0; i < a.numConvex; i++) {
            if(b.Inside(a.iterConv(i))||b.On_Convex(a.iterConv(i))) {m[n] = a.iterConv(i); n++;}
            }
        for(int i = 0; i < b.numConvex; i++) {
                if(a.Inside(b.iterConv(i))||a.On_Convex(b.iterConv(i))) {m[n] = b.iterConv(i); n++;}
        }
        for(int i = 0; i < a.numConvex; i++) {
          for(int j = 0; j < b.numConvex; j++) {
      if(fabs(R2Vector::signed_area(a.iterConv(i + 1) - a.iterConv(i),b.iterConv(j) - b.iterConv(j + 1))) > R2_EPSILON) {
        p = a.iterConv(i) + (a.iterConv(i + 1) - a.iterConv(i))*(R2Vector::signed_area(b.iterConv(j) - a.iterConv(i),b.iterConv(j) - b.iterConv(j + 1))/R2Vector::signed_area(a.iterConv(i + 1) - a.iterConv(i),b.iterConv(j) - b.iterConv(j + 1)));
       if(p.between(a.iterConv(i),a.iterConv(i + 1)) && p.between(b.iterConv(j),b.iterConv(j + 1))) {m[n] = p; n++; }
           }
         }
        }
        } ConvexHull r(m,n); if(n > 0) delete[] m;
        return r;
    }
//==================================================================================================

 //if(i != q)  v(a[i] - a[q]);  //C=-1*B*nodeY[nminX]-A*nodeX[nminX]; A = a[i].y - a[q].y; B = a[q].x - a[i].x; cos < A/sqrt(A*A+B*B) cos = A/sqrt(A*A+B*B);       
        

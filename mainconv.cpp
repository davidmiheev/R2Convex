
#include "R2.h"


int main()  {
  try {
    
    CBMP32x p(1000,1000,255,255,255);
    
    Paint b(p,-30,30,-30,30);
    
    int n = 0,n1 = 0,n2 = 0;
    
    double *m,*m1,*m2, r; 
    
    FILE *f;
    
    f = fopen("data.txt","r");
    while(fscanf(f,"%lf",&r) != EOF) n++;
    rewind(f);
    
    m = new double[n];
    for(int i = 0; fscanf(f,"%lf",&r) != EOF; m[i] = r, i++); 
    fclose(f);
    
    f = fopen("data1.txt","r");
    while(fscanf(f,"%lf",&r) != EOF) n1++;
    rewind(f);
    m1 = new double[n1];
    for(int i = 0; fscanf(f,"%lf",&r) != EOF; m1[i] = r, i++);
    fclose(f);
    
    f = fopen("data2.txt","r");
    while(fscanf(f,"%lf",&r) != EOF) n2++;
    rewind(f);
    m2 = new double[n2];
    for(int i = 0; fscanf(f,"%lf",&r) != EOF; m2[i] = r, i++);
    fclose(f);
    
    R2Point *d,*d1,*d2; 
    ConvexHull *c,c2;
    
    c = new ConvexHull[3];
    
    d = new R2Point[n/2]; 
    d1 = new R2Point[n1/2]; 
    d2 = new R2Point[n2/2];
    
    for(int i = 0, j = 0; i < n; d[j] = R2Point(m[i], m[i + 1]), i += 2, j++);
    //for(int i = 0; i < n/2; b.drawPointB(d[i]), i++);
     c[0] = ConvexHull(d,n/2);
    for(int i = 0, j = 0; i < n1; d1[j] = R2Point(m1[i], m1[i + 1]), i += 2, j++);
    
     c[1] = ConvexHull(d1,n1/2);
    for(int i = 0, j = 0; i < n2; d2[j] = R2Point(m2[i], m2[i + 1]), i += 2, j++);
   
     c[2] = ConvexHull(d2,n2/2);
    
    delete[] m; delete[] m1; delete[] m2;
    delete[] d; delete[] d1; delete[] d2;
    
    n = c[0].size(); n1 = c[1].size(); n2 = c[2].size();
    
    b.drawAxes();
    
    for(int i = 0; i < n; i++) b.drawLine(c[0].iterConv(i),c[0].iterConv(i + 1),1);
    for(int i = 0; i < n1; i++) b.drawLine(c[1].iterConv(i),c[1].iterConv(i + 1),1);
    for(int i = 0; i < n2; i++) b.drawLine(c[2].iterConv(i),c[2].iterConv(i + 1),1);
    
    c2 = ConvexHull::polyintersect(c,3); 
    Functor g(c2); //g(29.,29.) = 1; 
    
    n = g.c.size(); 
    delete[] c;
    
    for(int i = 0; i < n; i++) b.drawLine(g.c.iterConv(i),g.c.iterConv(i + 1),2);
    
    b.hatch(g.c);
    
    b.a.Save("res.bmp"); 
    n = 0;
    
    //printf("(%f,%f)\n",a.iterConv(-1).x,a.iterConv(-1).y);c2.Inside(R2Point(1,1))static_cast <bool>(g(1.,1.))
    
    if(g(1.,1.)) 
      printf("(1,1) inside Convex hull\n"); 
    else 
      printf("(1,1) outside Convex hull\n");
    
    printf("S = %f, P = %f\n", c2.square(), c2.perimeter());

    }
    catch(int j) {
        if(j == 1) printf("Error№1!\n");
        if(j == 2) printf("Error№2!\n");
        if(j == 3) printf("Error№3!\n");
    }


    return 0;
}


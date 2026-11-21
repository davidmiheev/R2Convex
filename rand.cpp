#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

static double frand() { return (double)rand() / RAND_MAX; }

int main(int argc, char** argv) {
      int counts[] = {15, 20, 16};
      unsigned seed = (unsigned)time(0);
      if(argc > 1) {
         int n = atoi(argv[1]);
         if(n <= 0) { fprintf(stderr, "usage: %s [points] [seed]\n", argv[0]); return 1; }
         counts[0] = counts[1] = counts[2] = n;
      }
      if(argc > 2) seed = (unsigned)strtoul(argv[2], 0, 10);
      srand(seed);
      const double PI = 3.14159265358979323846;
      const char* names[] = {"data.txt", "data1.txt", "data2.txt"};
      const double cx[] = {0., 6., -5.};
      const double cy[] = {0., 4., -4.};
      const double rx[] = {15., 16., 12.};
      const double ry[] = {15., 9., 12.};
      for(int k = 0; k < 3; k++) {
         FILE *f = fopen(names[k], "w");
         if(!f) { perror(names[k]); return 1; }
         for(int i = 0; i < counts[k]; i++) {
            double ang = 2.*PI*frand();
            double mod = (k == 2) ? 0.78 + 0.22*cos(5.*ang) : 1.; // 5-pointed star
            double rr = (0.85 + 0.3*frand()) * mod;
            fprintf(f,"%f\n%f\n", cx[k] + rx[k]*rr*cos(ang), cy[k] + ry[k]*rr*sin(ang));
         }
         fclose(f);
      }
 return 0;
}

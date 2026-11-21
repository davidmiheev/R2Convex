#include <stdlib.h>
#include <stdio.h>
#include <time.h>

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
      const char* names[] = {"data.txt", "data1.txt", "data2.txt"};
      for(int k = 0; k < 3; k++) {
         FILE *f = fopen(names[k], "w");
         if(!f) { perror(names[k]); return 1; }
         for(int i = 0; i < 2*counts[k]; i++) {
            fprintf(f,"%f\n", (rand() & 1 ? 1. : -1.) * rand() * 20. / RAND_MAX);
         }
         fclose(f);
      }
 return 0;
}

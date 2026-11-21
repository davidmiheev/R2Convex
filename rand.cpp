#include <stdlib.h>
#include <stdio.h>
#include <time.h>
int main() {
      srand((unsigned)time(0));
      const char* names[] = {"data.txt", "data1.txt", "data2.txt"};
      const int counts[] = {30, 40, 32};
      for(int k = 0; k < 3; k++) {
         FILE *f = fopen(names[k], "w");
         if(!f) { perror(names[k]); return 1; }
         for(int i = 0; i < counts[k]; i++) {
            fprintf(f,"%f\n", (rand() & 1 ? 1. : -1.) * rand() * 20. / RAND_MAX);
         }
         fclose(f);
      }
 return 0;
}

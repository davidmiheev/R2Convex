#include <stdlib.h>
#include <stdio.h>
int main() {
      FILE *f;
      f = fopen("data.txt","w");
      for(int i = 0; i < 30; i++) {
         if(i % 8 == 0||i % 8 == 1||i % 8 == 3||i % 8 == 6) fprintf(f,"%f\n", rand()*20./RAND_MAX);
         if(i % 8 == 2||i % 8 == 4||i % 8 == 5||i % 8 == 7) fprintf(f,"%f\n", rand()*-20./RAND_MAX);
        } fclose(f);
      f = fopen("data1.txt","w");
      for(int i = 0; i < 40; i++) {
         if(i % 8 == 0||i % 8 == 1||i % 8 == 3||i % 8 == 6) fprintf(f,"%f\n", rand()*20./RAND_MAX);
         if(i % 8 == 2||i % 8 == 4||i % 8 == 5||i % 8 == 7) fprintf(f,"%f\n", rand()*-20./RAND_MAX);
        } fclose(f);
      f = fopen("data2.txt","w");
      for(int i = 0; i < 32; i++) {
         if(i % 8 == 0||i % 8 == 1||i % 8 == 3||i % 8 == 6) fprintf(f,"%f\n", rand()*20./RAND_MAX);
         if(i % 8 == 2||i % 8 == 4||i % 8 == 5||i % 8 == 7) fprintf(f,"%f\n", rand()*-20./RAND_MAX);
        } fclose(f);
 return 0;
}

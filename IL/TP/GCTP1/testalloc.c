/*
 * a simple application to exercise malloc() and free() calls
 */

#include <stdlib.h>

#define MAXITERS   100000
#define MAXSIZE    1024

int main(int argc, char* argv[]) {
  float pfree = atof(argv[1]);
  int niter = rand() % MAXITERS;
  for(int i = 0; i < niter; i++ ) {
     int size = rand() % MAXSIZE;
     void* p = malloc(size * sizeof(char));
     float toss = (float)random() / RAND_MAX;
     if ( toss > pfree )
        free(p);
  }
  return 0;
}

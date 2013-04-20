#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ROUND_DOWN(x, s) ((x) & ~((s)-1))

double classify(double x, double c, double b);
inline
double o_classify(double x, double m, double w);

int main(int argc, char* argv[]) {
  clock_t t;
  int iN, N = 10000;
  int iM, M = atoi(argv[1]);

  double c = 0.5, b = 0.1;
  double m = c - b, w = 1/2.0/b;
  double x[N];
  double out[N];
						
  // Make runs repeatable for timing
  srand(1234);
  for (iN=0; iN<N; iN++)
    x[iN] = rand() / (double)RAND_MAX;
  // Start timing after initialization of x
  t = clock();

  const stepsize = 8;
  for (iM=0; iM<M; iM++) {
    for (iN=0; iN<ROUND_DOWN(N, stepsize); iN+=stepsize) {
      out[iN] = o_classify(x[iN], m, w);
      out[iN+1] = o_classify(x[iN+1], m, w);
      out[iN+2] = o_classify(x[iN+2], m, w);
      out[iN+3] = o_classify(x[iN+3], m, w);
      out[iN+4] = o_classify(x[iN+4], m, w);
      out[iN+5] = o_classify(x[iN+5], m, w);
      out[iN+6] = o_classify(x[iN+6], m, w);
      out[iN+7] = o_classify(x[iN+7], m, w);
    } 
    for (; iN<N; iN++)
      out[iN] = o_classify(x[iN], m, w);
  }

  t = clock() - t;
  printf ("Run took %f seconds.\n",((double)t)/CLOCKS_PER_SEC);

  #ifdef DEBUG
  for (iN=0; iN<10; iN++)
    printf("%f is classified as %f.\n", x[iN], out[iN]);
  #endif
}

double classify(double x, double c, double b) {
  if (x<(c-b))
    return 0;
  else if (x>(c+b))
    return 1;
  else {
    if (b>1E-7)
      return (x-c+b)/2/b;
    else
      return 0.5;
  } 
}

inline
double o_classify(double x, double m, double w) {
  double out = (x-m)*w;
  out = (out < 0) ? 0 : out;
  out = (out > 1) ? 1 : out;
  return out;
}

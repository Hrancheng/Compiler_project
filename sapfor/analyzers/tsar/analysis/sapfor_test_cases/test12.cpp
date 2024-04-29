
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define Max(A, B) ((A) > (B) ? (A) : (B))
#define MAXEPS 0.5

void init(int L, double (*restrict A)[L], double (*restrict B)[L]) {
  for (int I = 0; I < L; ++I) {
    for (int J = 0; J < L; ++J) {
      A[I][J] = 0;
      if (I == 0 || J == 0 || I == L - 1 || J == L - 1)
        B[I][J] = 0;
      else
        B[I][J] = 3 + I + J;
    }
  }
}

double iter(int L, double (*restrict A)[L], double (*restrict B)[L]) {
  double Eps = 0;
  for (int I = 1; I < L - 1; ++I) {
    for (int J = 1; J < L - 1; ++J) {
      double Tmp = fabs(B[I][J] - A[I][J]);
      Eps = Max(Tmp, Eps);
      A[I][J] = B[I][J];
    }
  }
  for (int I = 1; I < L - 1; ++I) {
    for (int J = 1; J < L - 1; ++J) {
      B[I][J] = (A[I - 1][J] + A[I + 1][J] + A[I][J - 1] + A[I][J + 1]) / 4.0;
    }
  }
  return Eps;
}

int main(int argc, char **argv) {
  int L = atoi(argv[1]);
  int ITMAX = atoi(argv[2]);
  double(*A)[L] = malloc(L * L * sizeof(double));
  double(*B)[L] = malloc(L * L * sizeof(double));
  init(L, A, B);
  for (int It = 1; It <= ITMAX; ++It) {
    double Eps = iter(L, A, B);
        printf("It=%4i Eps=%e
", It, Eps);
        if (Eps < MAXEPS)
            break;
  }
  free(A);
  free(B);
  return 0;
}

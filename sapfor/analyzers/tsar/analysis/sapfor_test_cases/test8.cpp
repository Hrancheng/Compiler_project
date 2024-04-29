
int foo(int I, long long X) { return *(int *)X = I; }

void bar(int N, int *X, float *restrict A) {
  int I;
  for (int I = 0; I < N; ++I) {
    A[I] = foo(I, (long long)X);
  }
}

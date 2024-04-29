
void foo(int N, double *restrict X, float *restrict A) {
  for (int I = 0; I < 10; ++I) {
    *X = I;
    A[I] = *X;
  }
}

int bar(int N, float *restrict A) {
  double X = N;
  foo(N, &X, A);
  return N;
}

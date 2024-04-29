
struct S {
  int *Y;
};

int foo(int I, struct S *restrict X) { return *X->Y = I; }

void bar(int N, struct S *restrict X, float *restrict A) {
  int I;
  for (int I = 0; I < N; ++I) {
    // X is a complex type with a pointer sub-type. So, we assume that
    // this call may have a side effect.
    A[I] = foo(I, X);
  }
}

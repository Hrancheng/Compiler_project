
int N;

void bar(int *X);

void foo(double *restrict A) {
  int X, Y, *P;
  X = 5;
  Y = 6;
  for (int I = 1; I < N - 1; ++I) {
    P = &X;
    Y = 5;
    P = &Y;
    A[I] = X + *P;
  }
  bar(&X);
  bar(&Y);
}

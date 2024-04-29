
double A[100];
int X;

void foo(int Flag) {
    for (int I = 0; I < 100; ++I) {
        if (Flag) {
            X = A[I];
        }
        A[I] = A[I] + 1;
        if (Flag) {
            A[I+1] = X;
        }
    }
}

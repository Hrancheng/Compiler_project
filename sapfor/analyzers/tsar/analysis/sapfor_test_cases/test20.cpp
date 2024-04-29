
int Z;

int foo() {
    int X, S;
    int *SPtr = &S;
    *SPtr = 1;
    for (int I = 0; I < 100; ++I) {
        int *XPtr = &X;
        *XPtr = I;
        S = S * X * Z;
    }
    return S;
}

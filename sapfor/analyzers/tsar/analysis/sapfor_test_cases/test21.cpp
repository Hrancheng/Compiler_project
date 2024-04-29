
int foo(int Y) {
    int I, X;
    for (I = 0; I < 10; ++I) {
        if (Y > 0) {
            X = I;
        }
        Y = X;
    }
    return Y;
}

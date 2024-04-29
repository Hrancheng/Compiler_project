
int foo(int Y) {
    int I, X;
    X = -5;
    for (I = 0; I < 10; ++I) {
        if (Y > 0) {
            X = I;
        }
        X = X + 1;
        Y = X;
    }
    return Y;
}

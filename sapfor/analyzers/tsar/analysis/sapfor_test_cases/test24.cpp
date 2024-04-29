
struct STy { int X; double Y; };

double foo(int N, struct STy S) {
    for (int I = 0; I < N; ++I) {
        S.Y += I;
        S.X *= I;
    }
    return S.Y + S.X;
}


void foo(int *restrict N, float *restrict A) {
    for (int I = 1; I < N[0]; I = I + 2) {
        float V = A[I];
        for (int J = N[I]; J < N[I + 1]; ++J) {
            if (J < N[I]) {
                A[J] = A[J] + V;
            } else {
                // The inner 'for' loop does not actually contain this expression.
                // So, this situation produces a special analysis case.
                // Thus analyzer may assume absence of data dependencies
                // in the inner loop.
                A[J - 1] = N[I];
                break;
            }
        }
    }
}

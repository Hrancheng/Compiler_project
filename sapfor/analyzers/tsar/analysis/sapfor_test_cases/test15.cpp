
void foo(int N, int M, int K, float *A) {
    for (int I = 0; I < K; ++I) {
        float V = A[I];
        for (int J = K; J < N; ++J) {
            if (J < M) {
                A[J] = A[J] + V;
            } else {
                // The inner 'for' loop does not actually contain this expression.
                // So, this situation produces a special analysis case.
                // Thus analyzer may assume absence of data dependencies
                // in the inner loop.
                A[J - 1] = M;
                break;
            }
        }
    }
}

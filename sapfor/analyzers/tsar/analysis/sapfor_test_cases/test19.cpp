
int test(int *a) {
    for (int j = 0; j < 2; ++j) {
        int b[3] = {1, 2, 3};
        b[j] *= 2;
        *a = b[j];
    }
    return *a;
}


int recur(int n) {
    if (n == 0)
        return 1;
    return recur(n - 1);
}

void foo() {
    recur(10);
}

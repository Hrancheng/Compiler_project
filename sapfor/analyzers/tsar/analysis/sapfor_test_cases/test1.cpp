
int one() {
    return 1;
}

void foo() {
    int I;
    for (I = 1; I < 10;) {
        I = I + one();
    }
}


void foo() {
    int I, *X = &I;
    for (I = 1; I < 10; I = I + *X);
}

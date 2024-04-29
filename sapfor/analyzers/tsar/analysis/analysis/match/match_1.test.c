int main() {
  int I;
#pragma sapfor analysis dependency(<I, 4>) explicitaccess(<I, 4>)
#pragma omp parallel default(shared)
  {
#pragma omp for
    for (I = 0; I < 10; ++I)
      ;
  }

  return 0;
}
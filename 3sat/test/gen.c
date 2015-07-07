#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <time.h>

int main(int argc, char *argv[]) {
  assert(("Usage: ./gen #clause #literal\n") && argc == 3);
  // N < 256, L < 2^64
  int N = atoi(argv[1]);
  long long int L = atoll(argv[2]);
  assert(("N < 256 && L < 2^64\n") && (N<256 && (L < exp2(64))));
  printf("%d %lld\n", N, L);
  int i, j;
  srand(time(NULL));
  for(i = 0; i < N; i++) {
    for(j = 0; j < 3; j++) {
      int sign = rand()%2;
      long long int num = rand()%L+1;
      num = (sign)?(-num):(num);
      printf("%lld ", num);
    }
    printf("\n");
  }
  
  return 0;
}

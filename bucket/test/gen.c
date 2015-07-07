#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#define LEN 8

int main(int argc, char *argv[]) {
  assert(("Usage: ./gen <item_number/94>\n") && argc == 2);
  long int N = atol(argv[1]);
  // 0x21 - 0x7E: (33,126)
  printf("%ld\n", N*94);
  while(N--) {
    int i, s;
    for(s=0;s<94;s++) {
      printf("%c", s+33);
      for(i=1;i<7;i++)
	printf("%c", rand()%93+33);
      printf("\n");
    }
  }
  
  return 0;
}

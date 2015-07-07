#include "test_function.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

int main(int argc, char *argv[]) {
  static const char *usage = "Usage:\n./main_test gen [gen_input_name] n M or\n,.main_test check [sequential_output] [concurrent_output]";
  static const char *gen = "gen";
  static const char *check = "check";
  
  if(strcmp(argv[1], gen)==0) {
    if(argc != 5) perror(usage);
    int n = atoi(argv[3]);
    int M = atoi(argv[4]);
    assert(("invalid n or M\n" && n>0 && n<=1024 && M>0 && M<(1<<20)));
    gen_input(argv[2], n, M);
  }
  else if(strcmp(argv[1], check)==0) {
    if(argc != 4) perror(usage);
    check_answer(argv[2], argv[3]);
  }
  else perror(usage);

  return 0;
}

#include "test_function.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

void gen_input(char *filename, int n, int M) {
  FILE *fgen = fopen(filename, "w+");
  assert(("n <= 1024, M <= 10^6\n" && n<=1024 && M <= 1<<20));
  fprintf(fgen, "%d %d\n", n, M);
  for(int i=0;i<n;i++)
    fprintf(fgen, "%d %d\n", rand()%1025, rand()%1025);
  printf("[input generation: done]\n");
  fclose(fgen);
}

void check_answer(char *seqFile, char *conFile) {
  FILE *fcon = fopen(conFile, "r");
  FILE *fseq = fopen(seqFile, "r");

  assert(("Failed to open/create sequential/concurrent output file\n" && fcon!=NULL && fseq!=NULL));
  
  int ans;
  fscanf(fseq, "%d", &ans);
  int thread_num, tans;
  while(fscanf(fcon, "%d%d", &thread_num, &tans)!=EOF) {
    if(ans == tans) printf("thread_num=%d: correct\n", thread_num);
    else printf("thread_num=%d: wrong, expected %d, but %d\n", thread_num, ans, tans);
  }
  fclose(fcon);
  fclose(fseq);
}

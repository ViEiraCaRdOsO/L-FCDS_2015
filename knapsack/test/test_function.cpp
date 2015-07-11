#include "test_function.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define EPS 1e-6

void gen_input(char *filename, int n, int M) {
  FILE *fgen = fopen(filename, "w+");
  assert(("n <= 1024, M <= 10^6\n" && n<=1024 && M <= 1<<20));
  fprintf(fgen, "%d %d\n", n, M);
  for(int i=0;i<n;i++)
    fprintf(fgen, "%d %d\n", rand()%1025, rand()%1025);
  printf("[input generation: done]\n");
  fclose(fgen);
}

void check_answer(char *seqFile, char *conFile, char *timeFile) {
  FILE *fcon = fopen(conFile, "r");
  FILE *fseq = fopen(seqFile, "r");
  FILE *ftime = fopen(timeFile, "r");

  assert(("Failed to open/create answer/to-check/time-measure file\n" && fcon!=NULL && fseq!=NULL && ftime!=NULL));
  
  int ans, tans;
  fscanf(fseq, "%d", &ans);
  fscanf(fcon, "%d", &tans);
  if(ans == tans) {
    double tseq, tcon;
    fscanf(ftime, "%lf", &tseq);
    while(fscanf(ftime, "%lf", &tcon) != EOF);
    printf("Correct | speedup = %.4lf, seq: %.4lf, con: %.4lf\n", tseq/(tcon+EPS), tseq, tcon);
  } else printf("Wrong Answer, expected %d, but %d\n", ans, tans);
    
  fclose(fcon);
  fclose(fseq);
  fclose(ftime);
}

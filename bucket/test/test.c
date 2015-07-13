#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define LENGTH 8
#define EPS 1e-6

FILE *fin1, *fin2, *ftime;

void openfiles(char* in1, char *in2, char *time) {
  fin1 = fopen(in1, "r");
  if (fin1 == NULL) {
    perror("fopen fin");
    exit(EXIT_FAILURE);
  }
  fin2 = fopen(in2, "r");
  if (fin2 == NULL) {
    perror("fopen fin");
    exit(EXIT_FAILURE);
  }
  ftime = fopen(time, "r");
  if (ftime == NULL) {
    perror("fopen ftime");
    exit(EXIT_FAILURE);
  }
}

void closefiles(void) {
  fclose(fin1);
  fclose(fin2);
  fclose(ftime);
}

int main(int argc, char* argv[]) {

  assert(("Usage: ./test [answer file] [to_check file] [time_measure file]\n") && argc == 4);
  
  openfiles(argv[1], argv[2], argv[3]);

  int correct = 1;
  char s1[LENGTH], s2[LENGTH];
  long int c;
  c = 0;
  while(~fscanf(fin1, "%s", s1) && ~fscanf(fin2, "%s", s2)) {
    if (strcmp(s1, s2) != 0) {
      correct = 0;
      break;
    }
    c++;
  }
  
  if (correct) {
    double tseq, tcon, tmp;
    tseq = tcon = tmp = 0;
    fscanf(ftime, "%lf", &tseq);
    while(fscanf(ftime, "%lf", &tmp) != EOF) tcon = tmp;
    printf("Correct | speedup = %.4lf, seq: %.4lf, con: %.4lf\n", tseq/(tcon+EPS), tseq, tcon);
  } else printf("Wrong answer at %ld: expected %s but %s\n", c, s1, s2);
  
  closefiles();
  
  return 0;
}

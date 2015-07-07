#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define LENGTH 8

FILE *fin1, *fin2;

void openfiles(char* in1, char *in2) {
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
}

void closefiles(void) {
  fclose(fin1);
  fclose(fin2);
}

int main(int argc, char* argv[]) {

  assert(("Usage: ./test <answer> <to_check>\n") && argc == 3);
  
  openfiles(argv[1], argv[2]);

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

  if (correct) printf("correct!\n");
  else printf("wrong answer at %ld: expected %s but %s\n", c, s1, s2);
  
  closefiles();
  
  return 0;
}

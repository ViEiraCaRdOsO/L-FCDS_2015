#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define EPS 1e-6

FILE *fin1, *fin2, *ftime;

int *pixel1, *pixel2;

void openfiles(char *f1, char *f2, char *time);
int read_compare(void);
void closefiles(void);

int main(int argc, char *argv[]) {
  assert(("Usage: ./check [answer-file] [to-check-file] [time_measure_file]\n") && argc == 4);
  openfiles(argv[1], argv[2], argv[3]);
  int rst = read_compare();
  if(rst == 1) {
    double tseq, tcon, tmp;
    fscanf(ftime, "%lf", &tseq);
    while(fscanf(ftime, "%lf", &tmp) != EOF) tcon = tmp;
    printf("Correct | speedup = %.4lf, seq: %.4lf, con: %.4lf\n", tseq/(tcon+EPS), tseq, tcon);
  }
  else puts("Wrong answer");
  closefiles();
  
  return 0;
}

void openfiles(char *f1, char *f2, char *time) {
  fin1 = fopen(f1, "r");
  if (fin1 == NULL) {
    perror("fopen answer-file");
    exit(EXIT_FAILURE);
  }
  fin2 = fopen(f2, "r");
  if (fin2 == NULL) {
    perror("fopen to-check-file");
    exit(EXIT_FAILURE);
  }
  ftime = fopen(time, "r");
  if (ftime == NULL) {
    perror("fopen time-measure-file");
    exit(EXIT_FAILURE);
  }
}

void closefiles(void) {
  fclose(fin1);
  fclose(fin2);
  fclose(ftime);
}

int read_compare(void) {
  long long int s1, s2;
  fread(&s1, sizeof(s1), 1, fin1);
  fread(&s2, sizeof(s2), 1, fin2);
  if (s1 != s2) return -1;
  pixel1 = (int *) malloc(s1 * s1 * sizeof(int));
  pixel2 = (int *) malloc(s2 * s2 * sizeof(int));
  
  if (!fread(pixel1, s1 * s1 * sizeof(int), 1, fin1)) {
    perror("read seq error");
    exit(EXIT_FAILURE);
  }
  if (!fread(pixel2, s2 * s2 * sizeof(int), 1, fin2)) {
    perror("read con error");
    exit(EXIT_FAILURE);
  }
  for (long long int y = 0; y < s1; y++)
    for (long long int x = 0; x < s1; x++)
      if (pixel1[y*s1+x] != pixel2[y*s2+x]) {
	free(pixel1);free(pixel2);
	return -1;
      }
  free(pixel1);free(pixel2);
  return 1;
}

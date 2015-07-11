#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <omp.h>
#include "bucketsort.h"

#define LENGTH 8

FILE *fin, *fout, *ftime;

char *strings;
long int N;

void openfiles(char* in, char* out, char* time) {
  fin = fopen(in, "r+");
  if (fin == NULL) {
    perror("fopen fin");
    exit(EXIT_FAILURE);
  }

  fout = fopen(out, "w");
  if (fout == NULL) {
    perror("fopen fout");
    exit(EXIT_FAILURE);
  }
  ftime = fopen(time, "a+");
  if (ftime == NULL) {
    perror("fopen ftime");
    exit(EXIT_FAILURE);
  }
}

void closefiles(void) {
  fclose(fin);
  fclose(fout);
  fclose(ftime);
}

int main(int argc, char* argv[]) {
  assert(("Usage: ./main [input_file] [output_file] [time_measure_file]\n") && argc == 4);
  openfiles(argv[1], argv[2], argv[3]);
	
  long int i, *r;
  fscanf(fin, "%ld", &N);
  strings = (char*) malloc(N * LENGTH);
  if (strings == NULL) {
    perror("malloc strings");
    exit(EXIT_FAILURE);
  }

  for (i = 0; i < N; i++) fscanf(fin, "%s", strings + (i * LENGTH));

  fflush(stdout);

  double st = omp_get_wtime();
  r = bucket_sort(strings, LENGTH, N);
  double ed = omp_get_wtime();

  printf("Take %.4lf s\n", ed-st);
  fprintf(ftime, "%.4lf\n", ed-st);
	
  fflush(stdout);
	
  for (i = 0; i < N; i++)
    fprintf(fout, "%s\n", strings + (r[i] * LENGTH));

  free(r);
  free(strings);
  closefiles();
	
  return EXIT_SUCCESS;
}

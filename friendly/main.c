#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <omp.h>
#include "friendly.h"

FILE *ftime;

void openfiles(char* time) {
  ftime = fopen(time, "a+");
  if (ftime == NULL) {
    perror("fopen: cannot create/open time measure file");
    exit(EXIT_FAILURE);
  }
}

void closefiles(void) {
  fclose(ftime);
}

int main(int argc, char **argv) {
  assert(("Usage ./main [time_measure file]") && argc == 2);
  openfiles(argv[1]);
  
  long int start;
  long int end;

  while (1) {
    scanf("%ld %ld", &start, &end);
    if (start == 0 && end == 0)
      break;
    double st = omp_get_wtime();
    printf("Number %ld to %ld\n", start, end);
    friendly_numbers(start, end);
    double ed = omp_get_wtime();
    fprintf(ftime, "%.4lf ", ed - st);
  }

  fprintf(ftime, "\n");

  closefiles();

  return EXIT_SUCCESS;
}

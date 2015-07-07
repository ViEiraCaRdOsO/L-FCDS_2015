#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

#define CHUNK 1000

long solveClauses(short **clauses, int nClauses, int nVar) {
  long *iVar = (long *) malloc(nVar * sizeof(long));
  int i;
  for (i = 0; i < nVar; i++)
    iVar[i] = exp2(i);

  unsigned long maxNumber = exp2(nVar);
  unsigned long number;
  short var;
  int c;
  int found = -1;
#pragma omp parallel for schedule(dynamic, CHUNK) private(var, c)
  for (number = 0; number < maxNumber; number++) {
    if(found == -1) {
      for (c = 0; c < nClauses; c++) {
	var = clauses[0][c];
	if (var > 0 && (number & iVar[var - 1]) > 0)
	  continue; // clause is true
	else if (var < 0 && (number & iVar[-var - 1]) == 0)
	  continue; // clause is true

	var = clauses[1][c];
	if (var > 0 && (number & iVar[var - 1]) > 0)
	  continue; // clause is true
	else if (var < 0 && (number & iVar[-var - 1]) == 0)
	  continue; // clause is true

	var = clauses[2][c];
	if (var > 0 && (number & iVar[var - 1]) > 0)
	  continue; // clause is true
	else if (var < 0 && (number & iVar[-var - 1]) == 0)
	  continue; // clause is true

	break; // clause is false
      }

      if (c == nClauses) {
	found = number;
      }
    }
  }
  return found;
}

short **readClauses(int nClauses, int nVar) {

  short **clauses = (short **) malloc(3 * sizeof(short *));
  clauses[0] = (short *) malloc(nClauses * sizeof(short));
  clauses[1] = (short *) malloc(nClauses * sizeof(short));
  clauses[2] = (short *) malloc(nClauses * sizeof(short));

  int i;
  for (i = 0; i < nClauses; i++) {
    scanf("%hd %hd %hd", &clauses[0][i], &clauses[1][i], &clauses[2][i]);
  }

  return clauses;
}

int main(int argc, char *argv[]) {

  int nClauses;
  int nVar;

  scanf("%d %d", &nClauses, &nVar);

  short **clauses = readClauses(nClauses, nVar);

  double st = omp_get_wtime();
  long solution = solveClauses(clauses, nClauses, nVar);
  double ed = omp_get_wtime();
  printf("take %lfs\n", ed-st);
  
  int i;
  if (solution >= 0) {
    printf("Solution found [%ld]: ", solution);
    for (i = 0; i < nVar; i++)
      printf("%d ", (int) ((solution & (long) exp2(i)) / exp2(i)));
    printf("\n");
  } else
    printf("Solution not found.\n");

  return EXIT_SUCCESS;
}


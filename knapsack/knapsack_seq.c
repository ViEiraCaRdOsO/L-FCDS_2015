/* @author St\'efano Drimon Kurz M\'or */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <omp.h>

typedef struct _item_t {
  int value;   // v_i
  int weight;  // w_i
  float density; // v_i/w_i
} item_t;

FILE *fin, *fout, *ftime;

int greater_f(const int x, const int y);
int compare_f(const void *x, const void *y);
void openfiles(char* in, char* out, char *time);
void closefiles();
int knapsack_f(const int n, const int M, const item_t * const itens) {

  int v = 0, w = 0;
  int r = 0;

  if (n < 1) return 0;

  while (M - w >= 0) {
    r = greater_f(r, v + knapsack_f(n - 1, M - w, &(itens[1])));
    v += itens[0].value;
    w += itens[0].weight;
  }

  return r;
}

int main(int argc, char *argv[]) {
  assert(("Usage: ./knapsack.c [input file] [output file] [time_measure file]\n" && argc == 4));
  openfiles(argv[1], argv[2], argv[3]);

  int n, M;
  item_t *itens;
  int i;

  fscanf(fin, "%d %d", &n, &M);
  itens = (item_t*) calloc(n, sizeof(item_t));

  for (i = 0; i < n; ++i) {
    fscanf(fin, "%d %d", &(itens[i].value), &(itens[i].weight));
    itens[i].density = (float) (itens[i].value) / itens[i].weight;
  }

  double st = omp_get_wtime();
  qsort(itens, (size_t) n, sizeof(item_t), compare_f);
  fprintf(fout, "%d\n", knapsack_f(n, M, itens));
  double ed = omp_get_wtime();
  free(itens);

  fprintf(ftime, "%.4lf\n", ed - st);
  
  closefiles();
  
  return 0;
}

int greater_f(const int x, const int y) {
  return (x > y) ? x : y;
}

int compare_f(const void *x, const void *y) {
  return ((item_t*) x)->density - ((item_t*) y)->density;
}

void openfiles(char* in, char* out, char *time) {
  fin = fopen(in, "r");
  if(fin == NULL) {
    perror("fopen: input file");
    exit(EXIT_FAILURE);
  }
  fout = fopen(out, "w+");
  if(fout == NULL) {
    perror("fopen: output file");
    exit(EXIT_FAILURE);
  }
  ftime = fopen(time, "w+");
  if(ftime == NULL) {
    perror("fopen: time-measure file");
    exit(EXIT_FAILURE);
  }
}

void closefiles() {
  fclose(fin);
  fclose(fout);
  fclose(ftime);
}

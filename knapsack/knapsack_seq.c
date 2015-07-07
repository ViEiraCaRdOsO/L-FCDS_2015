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

int greater_f(const int x, const int y);
int compare_f(const void *x, const void *y);

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

int main(int argc, const char *argv[]) {
  assert(("Usage: ./knapsack.c <inputFile> <outputFile>\n" && argc == 3));
  FILE *fin, *fout;
  fin = freopen(argv[1], "r", stdin);
  fout = freopen(argv[2], "w+", stdout);
  assert(("Failed to create inputfile/outfie!\n" && fin!=NULL && fout!=NULL));

  int n, M;
  item_t *itens;
  int i;

  scanf("%d %d", &n, &M);
  itens = (item_t*) calloc(n, sizeof(item_t));

  for (i = 0; i < n; ++i) {
    scanf("%d %d", &(itens[i].value), &(itens[i].weight));
    itens[i].density = (float) (itens[i].value) / itens[i].weight;
  }

  qsort(itens, (size_t) n, sizeof(item_t), compare_f);

  printf("%d\n", knapsack_f(n, M, itens));

  free(itens);

  fclose(fin);
  fclose(fout);
  
  return 0;
}

int greater_f(const int x, const int y) {
  return (x > y) ? x : y;
}

int compare_f(const void *x, const void *y) {
  return ((item_t*) x)->density - ((item_t*) y)->density;
}


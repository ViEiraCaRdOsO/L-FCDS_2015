#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include <assert.h>

#define max(x,y) ((x)>(y)?(x):(y))

#define mMAX 1000001
#define nMAX 1025

FILE *fin, *fout, *ftime;

int value[nMAX];
int weight[nMAX];
long long int dp[mMAX];
omp_lock_t lock[mMAX];

void openfiles(char* in, char* out, char *time);
void closefiles();

int main(int argc, char *argv[]) {
  assert(("Usage: ./knapsack.c [input file] [output file] [time_measure file]\n" && argc == 4));
  openfiles(argv[1], argv[2], argv[3]);
  
  int n, M;
  fscanf(fin, "%d %d", &n, &M);

  for(int i=0;i<n;i++) fscanf(fin, "%d%d", &value[i], &weight[i]);
  for(int i=0;i<=M;i++) {
    dp[i] = 0;
    omp_init_lock(&lock[i]);
  }

  double start_time = omp_get_wtime();
  
  // knapsack dynamic programming
#pragma omp parallel for shared(dp)
  for(int i=0;i<n;i++)
    for(int j=weight[i];j<=M;j++) {
      // fine-grained lock for dp[j]
      omp_set_lock(&lock[j]);
      dp[j] = max(dp[j], dp[j-weight[i]]+value[i]);
      omp_unset_lock(&lock[j]);
    }

  double end_time = omp_get_wtime();

  printf("take %.4lf s, ", end_time - start_time);
  fprintf(ftime, "%.4lf\n", end_time - start_time);
  
  printf("%lld\n", dp[M]);
  fprintf(fout, "%lld\n", dp[M]);

  closefiles();
  
  return 0;
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
  ftime = fopen(time, "a+");
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include <assert.h>

#define max(x,y) ((x)>(y)?(x):(y))

#define mMAX 1000001
#define nMAX 1025

FILE *fin, *fout;

int value[nMAX];
int weight[nMAX];
long long int dp[mMAX];
omp_lock_t lock[mMAX];

void openfiles(char* in, char* out);
void closefiles();

int main(int argc, char *argv[]) {

  assert(("Usage: ./knapsack <num_thread> <input_file> <output_file>\n" && argc == 4));
  int num_threads = atoi(argv[1]);
  omp_set_num_threads(num_threads);
  assert(("Length of input/output file name should not exceed 100\n" && strlen(argv[2])<100 && strlen(argv[3])<100));
  
  openfiles(argv[2], argv[3]);
  
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

  printf("using %d threads, take %.4lf s, ", num_threads, end_time - start_time);
  
  printf("%lld\n", dp[M]);
  fprintf(fout, "%d %lld\n", num_threads, dp[M]);

  closefiles();
  
  return 0;
}

void openfiles(char* in, char* out) {
  fin = fopen(in, "r");
  if(fin == NULL) {
    perror("input file not exist!\n");
    exit(EXIT_FAILURE);
  }
  
  fout = fopen(out, "a");
  if(fout == NULL) fout = fopen(out, "w+");
  if(fout == NULL) {
    perror("cannot create output file\n");
    exit(EXIT_FAILURE);
  }
}

void closefiles() {
  fclose(fin);
  fclose(fout);
}

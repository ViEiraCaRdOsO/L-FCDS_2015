#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include "bucketsort.h"

#define N_BUCKETS 94

typedef struct {
  long int *data;
  int length;
  long int total;
} bucket;

// if memory is not large enough
void inplace_merge(char *a, bucket *dst, bucket *src) {
  
}

void non_inplace_merge(char *a, bucket *dst, bucket *src) {
  long int l1 = dst->total;
  long int l2 = src->total;
  long int l = l1 + l2;
  long int *t = malloc(sizeof(long int) * (l));
  long int t1, t2, tn;
  int length = dst->length;
  t1 = t2 = tn = 0;
  while ((t1 < l1) && (t2 < l2)) {
    if (strcmp(a + dst->data[t1] * length, a + src->data[t2] * length) < 0) t[tn++] = dst->data[t1++];
    else t[tn++] = src->data[t2++];
  }

  while (t1 < l1) t[tn++] = dst->data[t1++];

  while (t2 < l2) t[tn++] = src->data[t2++];

  // copy to destination
  dst->total = l;
  for (tn = 0; tn < l; tn++)
    dst->data[tn] = t[tn];
}

void sort(char *a, bucket *bucket) {
  int j, i, length;
  long int key;
  length = bucket->length;
  for (j = 1; j < bucket->total; j++) {
    key = bucket->data[j];
    i = j - 1;
    while (i >= 0 && strcmp(a + bucket->data[i] * length, a + key * length) > 0) {
      bucket->data[i + 1] = bucket->data[i];
      i--;
    }
    bucket->data[i + 1] = key;
  }
}

long int* bucket_sort(char *a, int length, long int size) {

  long int i;
  bucket buckets[N_BUCKETS], *b;
  long int *returns;
  omp_lock_t lock[N_BUCKETS];
  
  // allocate memory
  returns = malloc(sizeof(long int) * size);
  for (i = 0; i < N_BUCKETS; i++) {
    buckets[i].data = returns + i * size / N_BUCKETS;
    buckets[i].length = length;
    buckets[i].total = 0;
    omp_init_lock(&lock[i]);
  }

#pragma omp parallel
{
  bucket local_buckets[N_BUCKETS];
  long int *local_data;
  long int j;
  int thread_num = omp_get_num_threads();
  int id = omp_get_thread_num();
  local_data = malloc(sizeof(long int) * size);
  
  // allocate buffer
  for (j = 0; j < N_BUCKETS; j++) {
    local_buckets[j].data = local_data + j * size / N_BUCKETS;
    local_buckets[j].length = length;
    local_buckets[j].total = 0;
  }

  long int split = (size+thread_num-1) / thread_num;
  long int st = id * split;
  long int ed = st + split;
  ed = (ed < size) ? ed : size;

  // copy the keys to local_buckets
  for (j = st; j < ed; j++) {
    b = &local_buckets[*(a + j * length) - 0x21];
    b->data[b->total++] = j;
  }
  
  // sort each local_buckets
  for (j = 0; j < N_BUCKETS; j++)
    sort(a, &local_buckets[j]);

#pragma omp barrier

  // start from its division and perform in-place merge to bucket
  int b_index = id * (N_BUCKETS / thread_num);
  for (j = 0;j < N_BUCKETS; j++) {
    omp_set_lock(&lock[b_index]);
    non_inplace_merge(a, &buckets[b_index], &local_buckets[b_index]);
    omp_unset_lock(&lock[b_index]);
    b_index = (b_index+1)%N_BUCKETS;
  }
}
  
  return returns;
}

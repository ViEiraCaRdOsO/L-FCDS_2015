#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#include "friendly.h"

#define CHUNK 100000

int gcd(int u, int v) {
  while((u%=v)&&(v%=u));
  return u+v;
}

void friendly_numbers(long int start, long int end) {
  long int last = end - start +1;
  long int *the_num;
  the_num = (long int*) malloc(sizeof(long int)*last);
  long int *num;
  num = (long int*) malloc(sizeof(long int)*last);
  long int *den;
  den = (long int*) malloc(sizeof(long int)*last);

  long int i,j, factor,ii,sum,done,n;
  
#pragma omp parallel for schedule(dynamic, CHUNK) private(factor, ii, sum, done, n)
  for(i = start; i <= end; i++) {
    ii = i - start;
    sum = 1 + i;
    the_num[ii] = i;
    done = i;
    factor = 2;
    while(factor < done) {
      if ((i % factor) == 0) {
	sum += (factor + (i/factor));
	if ((done = i/factor) == factor) sum -= factor;
      }
      factor++;
    }
    num[ii] = sum;
    den[ii] = i;
    n = gcd(num[ii], den[ii]);
    num[ii] /= n;
    den[ii] /= n;
  }

#pragma omp parallel for schedule(dynamic, CHUNK) private(j)
  for(i = 0; i < last; i++) {
    for (j = i+1; j < last; j++) {
      if ((num[i] == num[j]) && (den[i] == den[j]))
	printf("%ld and %ld are FRIENDLY\n", the_num[i], the_num[j]);
    }
  }
  
  free(the_num);
  free(num);
  free(den);
}

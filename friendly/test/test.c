#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>

#define STR_MAX 100
#define MAX 500001
#define TEST_MAX 10000
#define EPS 1e-6

typedef struct pair{
  long int a;
  long int b;
}pair;

FILE *fseq, *fcon, *ftime;

int cmp(const void *a, const void *b) {
  pair c = *(pair *)a;
  pair d = *(pair *)b;
  if(c.a != d.a)
    return c.a - d.a;
  return c.b - d.b;
}

void openfiles(char *seq, char *con, char *time) {
  fseq = fopen(seq, "r");
  if(fseq == NULL) {
    perror("fopen fseq");
    exit(EXIT_FAILURE);
  }
  fcon = fopen(con, "r");
  if(fcon == NULL) {
    perror("fopen fcon");
    exit(EXIT_FAILURE);
  }
  ftime = fopen(time, "r");
  if(ftime == NULL) {
    perror("fopen ftime");
    exit(EXIT_FAILURE);
  }
}

void closefiles() {
  fclose(fseq);
  fclose(fcon);
  fclose(ftime);
}

int main(int argc, char *argv[]) {
  assert(("Usage: ./test [seq_out] [con_out] [time_measure]") && argc == 4);
  openfiles(argv[1], argv[2], argv[3]);

  char line[STR_MAX];
  pair Aseq[MAX];
  pair Acon[MAX];
  int i,j;
  long int sS,sE, cS, cE;
  sS = sE = cS = cE = -1;
  long int sn, cn;
  int sf, cf, tn;
  sf = cf = tn = 1;
  int correct = 1;
  while(correct) {
    sn = cn = 0;
    while(fgets(line, STR_MAX, fseq) != NULL) {
      if(line[0]=='N') {
	sscanf(line, "Number %ld to %ld", &sS, &sE);
	if(sf) sf = 0;
	else break;
      } else if(isdigit(line[0])) {
	sscanf(line, "%ld and %ld are FRIENDLYd", &Aseq[sn].a, &Aseq[sn].b);
	sn++;
      } else break;
    }

    if(sn == 0) break;
    
    while(fgets(line, STR_MAX, fcon) != NULL) {
      if(line[0]=='N') {
	sscanf(line, "Number %ld to %ld", &cS, &cE);
	if(cf) cf = 0;
	else break;
      } else if(isdigit(line[0])) {
	sscanf(line, "%ld and %ld are FRIENDLY", &Acon[cn].a, &Acon[cn].b);
	cn++;
      } else break;
    }

    if(sS != cS || sE != cE) {
      perror("missing test case!\n");
      correct = 0;
      break;
    }

    if(sn != cn) {
      correct = 0;break;
    }

    qsort(Aseq, sn, sizeof(pair), cmp);
    qsort(Acon, cn, sizeof(pair), cmp);
    long int t;
    for(t = 0; t < sn; t++) {
      if(Aseq[t].a != Acon[t].a || Aseq[t].b != Acon[t].b) {
	correct = 0;break;
      }
    }
    sn = cn = 0;
    tn++;
  }

  double seq_time[tn], con_time[tn];
  for(i=0;i<tn;i++)
    fscanf(ftime, "%lf", &seq_time[i]);
  j = 0;
  while(fscanf(ftime, "%lf", &con_time[j%tn]) != EOF) j++;
  
  closefiles();

  if(correct) {
    printf("Correct | speedup = ");
    for(i=0;i<tn;i++) printf("%.4lf ", seq_time[i]/(EPS+con_time[i]));
    printf("\n");
  } else printf("Wrong Answer at %dth test case\n", tn);
  
  return 0;
}

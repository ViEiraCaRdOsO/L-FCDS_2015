#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>

#define STR_MAX 100
#define MAX 500001

typedef struct pair{
  long int a;
  long int b;
}pair;

FILE *fseq, *fcon;

int cmp(const void *a, const void *b) {
  pair c = *(pair *)a;
  pair d = *(pair *)b;
  if(c.a != d.a)
    return c.a - d.a;
  return c.b - d.b;
}

void openfiles(char *seq, char *con) {
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
}

void closefiles() {
  fclose(fseq);
  fclose(fcon);
}

int main(int argc, char *argv[]) {
  assert(("Usage: ./test <seq_out> <con_out>") && argc == 3);
  openfiles(argv[1], argv[2]);

  char line[STR_MAX];
  pair Aseq[MAX];
  pair Acon[MAX];
  long int sS,sE, cS, cE;
  sS = sE = cS = cE = -1;
  long int sn, cn;
  int sf, cf, tn;
  sf = cf = tn = 1;
  int correct = 1;
  double seq_time = -1;
  double con_time = -1;
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
    sscanf(line, "using time %lfs", &seq_time);

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
    sscanf(line, "using time %lfs", &con_time);

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

  closefiles();

  if(correct) {
    printf("Correct, speedup = %.4lf | seq: %.4lfs, con: %.4lfs\n", seq_time/con_time, seq_time, con_time);
  } else printf("Wrong Answer at %dth test case\n", tn);
  
  return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <omp.h>

#define MAX 200
#define EPS 1e-6

FILE *fseq, *fcon, *fgen, *ftime;
char ans_seq[MAX], ans_con[MAX];
int N, L;

void openfiles(char* seq, char *con, char *gen, char *time);
void closefiles(void);
void get_N_L(void);

int main(int argc, char *argv[]) {

  assert(("Usage: ./check [gen.in] [seq.out] [con.out] [time.out]") && argc == 5);
  openfiles(argv[1], argv[2], argv[3], argv[4]);
  fscanf(fseq, "%s", ans_seq);
  fscanf(fcon, "%s", ans_con);
  if (strcmp("Solution not found.", ans_seq) == 0) {
    if (strcmp("Solution not found.", ans_con) != 0) printf("Wrong answer, expected: %s, but %s\n", ans_seq, ans_con);
  } else {
    // solution found, check if con.out is a legal answer
    get_N_L();
    int conL[MAX];
    for (int i=0;i<L;i++) fscanf(fcon, "%d", &conL[i]);
    short t[3];
    for (int i=0;i<N;i++) {
      for (int j=0;j<3;j++) fscanf(fgen, "%hd", &t[j]);
      bool ok = false;
      for (int j=0;j<3;j++) {
	if ((t[j]>0 && conL[t[j]]==1) || (t[j]<0 && conL[-t[j]]==0)) {
	  ok = true;break;
	}
      }
      if (!ok) {
	printf("Wrong answer, con.out is not a legal answer\n");
	return 0;
      }
    }
  }

  double tseq, tcon, tmp;
  tseq = tcon = tmp = 0;
  fscanf(ftime, "%lf", &tseq);
  while(fscanf(ftime, "%lf", &tmp) != EOF) tcon = tmp;
  printf("Correct | speedup = %.4lf, seq: %.4lf, con: %.4lf\n", tseq/(tcon+EPS), tseq, tcon);
  
  closefiles();
  
  return 0;
}

void openfiles(char* seq, char *con, char *gen, char *time) {
  fseq = fopen(seq, "r");
  if (fseq == NULL) {
    perror("fopen: cannot open seq.out");
    exit(EXIT_FAILURE);
  }
  
  fcon = fopen(con, "r");
  if (con == NULL) {
    perror("fopen: cannot open con.out");
    exit(EXIT_FAILURE);
  }

  fgen = fopen(gen, "r");
  if (gen == NULL) {
    perror("fopen: cannot open gen.in");
    exit(EXIT_FAILURE);
  }

  ftime = fopen(time, "r");
  if (gen == NULL) {
    perror("fopen: cannot open time.out");
    exit(EXIT_FAILURE);
  }
}

void closefiles(void) {
  fclose(fseq);
  fclose(fcon);
  fclose(fgen);
  fclose(ftime);
}

void get_N_L(void) {
  fscanf(fgen, "%d", &N);
  fscanf(fgen, "%d", &L);
}

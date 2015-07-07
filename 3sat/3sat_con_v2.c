#include <iostream>
#include <random>
#include <ctime>
#include <cstdlib>
#include <map>
#include <cmath>
#include <omp.h>

#define cMAX 256
#define nTRIAL 100000000

using namespace std;

/*
random assignment, high false positive, don't use!
*/
typedef struct clause{
  long long int L[3];
}clause;

clause Clauses[cMAX];

map<long long int, int> ltrMap;
long long int ltrIndex[cMAX*3];
bool assignValue[cMAX*3];
int ln;
bool found;

void readClauses(int nClauses);
void printClauses(int nClauses);
int findLtr(long long int x);
void initLtr(int nClauses);
void random_assign(int nClauses);
int cmp(const void *a, const void *b) {
  return *(long long int *)a - *(long long int *)b;
}

int main(int argc, char *argv[]) {
  int nClauses;
  long long int nLiterals;
  
  cin >> nClauses >> nLiterals;
  readClauses(nClauses);

  //printClauses(nClauses);
  
  initLtr(nClauses);
  qsort(ltrIndex, ln, sizeof(long long int), cmp);
  #pragma omp parallel
  random_assign(nClauses);
  
  if(found) {
    cout << "Solution found :";
    for(int i=0;i<ln;i++)
      cout << " " << assignValue[ltrMap[ltrIndex[i]]];
    cout << endl;
  } else cout << "Solution not found!" << endl;
  
  return 0;
}

void readClauses(int nClauses) {
  for(int i=0;i<nClauses;i++)
    for(int j=0;j<3;j++)
      cin >> Clauses[i].L[j];
}

void printClauses(int nClauses) {
  for(int i=0;i<nClauses;i++) {
    for(int j=0;j<3;j++) cout << Clauses[i].L[j] << " ";
    cout << "\n";
  }
}

void initLtr(int nClauses) {
  found = false;
  ln = 0;
  map<long long int, int>::iterator it;
  for(int i=0;i<nClauses;i++) {
    for(int j=0;j<3;j++) {
      int t = abs(Clauses[i].L[j]);
      if(t) {
	it = ltrMap.find(t);
	if(it == ltrMap.end()) {
	  ltrMap.insert(pair<long long int, int>(t, ln));
	  ltrIndex[ln++] = t;
	}
      }
    }
  }
}

void random_assign(int nClauses) {
  int tn = omp_get_num_threads();
  long long int trial = nTRIAL/tn;
  int tmpAssign[ln];
  random_device rd;
  mt19937 gen(rd());
  uniform_int_distribution<> dis(1, 100);
  for(int i=0;i<ln;i++) tmpAssign[i] = 0;
  for(long long int i=0;i<trial && !found;i++) {
    int c;
    for(c=0;c<nClauses;c++) {
      bool ok = false;
      for(int j=0;j<3;j++) {
	int t = Clauses[c].L[j];
	int index = findLtr(t);
	if(tmpAssign[index] == 0) { // not yet assigned, assign a random value
	  
	  tmpAssign[index] = (dis(gen) > 50)?1:-1;
	}
	if(tmpAssign[index] == 1 && t > 0) ok = true;
	else if(tmpAssign[index] == -1 && t < 0) ok = true;
      }
      if(ok) continue;
      else break;
    }
    if(c == nClauses) {
      #pragma omp critical
      {
	found = true;
	for(int j=0;j<ln;j++)
	  assignValue[j] = (tmpAssign[j]==1)?true:false;
      }
      return;
    }
  }
}

int findLtr(long long int x) {
  map<long long int, int>::iterator it = ltrMap.find(abs(x));
  if(it != ltrMap.end())
    return it->second;
  return -1;
}

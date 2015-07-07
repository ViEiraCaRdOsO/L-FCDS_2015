#include <iostream>
#include <map>
#include <cstdlib>
#include <cmath>
#include <omp.h>

#define cMAX 256

using namespace std;

/* 
too slow! don't use it
*/
typedef struct clause{
  long long int L[3];
}clause;

typedef struct inC{
  int positive[cMAX];
  int negative[cMAX];
  int nP;
  int nN;
}inC;

clause Clauses[cMAX];
bool skipSet[cMAX];

map<long long int, int> refMap;
inC LiteralArray[cMAX*3];
int inCn;

map<long long int, int> ltrMap;
long long int ltrIndex[cMAX*3];
bool assignValue[cMAX*3];
int ln;
bool found;

void readClauses(int nClauses);
int findRefIndex(long long int x);
void printAssign(int ln);
void initLtr(int nClauses);
void Divide_Conquer(int nClauses, int depth, int *curAssign);
int cmp(const void *a, const void *b) {
  return *(long long int *)a - *(long long int *)b;
}

int main(int argc, char *argv[]) {
  int nClauses;
  long long int nLiterals;
  
  cin >> nClauses >> nLiterals;
  // read and eliminate literals and clauses
  readClauses(nClauses);

  //printClauses(nClauses);
  double st = omp_get_wtime();
  
  initLtr(nClauses);
  int init[ln]; // 0: unsigned, 1: true, -1: false
  for(int i=0;i<ln;i++) init[i]=0;
#pragma omp parallel
#pragma omp single
  Divide_Conquer(nClauses, 0, init);

  double ed = omp_get_wtime();
  cout << "take " << (ed-st) << "s" << endl;
  
  if(found) {
    cout << "Solution found :";
    qsort(ltrIndex, ln, sizeof(long long int), cmp);
    for(int i=0;i<ln;i++)
      cout << " " << assignValue[ltrMap[ltrIndex[i]]];
    cout << endl;
  } else cout << "Solution not found!" << endl;
  
  return 0;
}

void readClauses(int nClauses) {
  for(int i=0;i<nClauses;i++) skipSet[i] = false;
  inCn = 0;
  for(int i=0;i<nClauses;i++) {
    int x[3];
    for (int j=0;j<3;j++) cin >> x[j];
    for(int j=0;j<3;j++) {
      long long int tmp = abs(x[j]);
      int index = findRefIndex(tmp);
      // it's the first time for this literal to appear in clauses
      if(index == -1) {
	if(x[j]>0) {
	  LiteralArray[inCn].positive[0] = i;
	  LiteralArray[inCn].nP = 1;
	  LiteralArray[inCn].nN = 0;
	  refMap.insert(pair<long long int, int>(tmp, inCn));
	  inCn++;
	} else {
	  LiteralArray[inCn].negative[0] = i;
	  LiteralArray[inCn].nP = 0;
	  LiteralArray[inCn].nN = 1;
	  refMap.insert(pair<long long int, int>(tmp, inCn));
	  inCn++;
	}
      } else {
	// to see if we could eliminate literals
	if(x[j]>0) {
	  if(LiteralArray[index].nN > 0) { // check if there's contradiction in same clause
	    bool in = false;
	    for(int k=LiteralArray[index].nN-1;k>=0 && LiteralArray[index].negative[k]>=i;k--)
	      if(LiteralArray[index].negative[k] == i) {
		in = true;break;
	      }
	    if(in) { // this clause is satisfied
	      for(int k=0;k<j;k++) {
		if(x[k] > 0) LiteralArray[findRefIndex(abs(x[k]))].nP--;
		else if(x[k] < 0) LiteralArray[findRefIndex(abs(x[k]))].nN--;
	      }
	      skipSet[i] = true; // mark clauses as to skip
	      break;
	    }
	  }
	  
	  if(LiteralArray[index].nP > 0) { // eliminate duplicate variable
	    bool in = false;
	    for(int k=LiteralArray[index].nP-1;k>=0 && LiteralArray[index].positive[k]>=i;k--)
	      if(LiteralArray[index].positive[k] == i) {
		in = true;break;
	      }
	    if(in) x[j] = 0;
	    else LiteralArray[index].positive[LiteralArray[index].nP++] = i;
	  } else LiteralArray[index].positive[LiteralArray[index].nP++] = i;
	} else { // x[j] < 0
	  if(LiteralArray[index].nP > 0) { // check if there's contradiction in same clause
	    bool in = false;
	    for(int k=LiteralArray[index].nP-1;k>=0 && LiteralArray[index].positive[k]>=i;k--)
	      if(LiteralArray[index].positive[k] == i) {
		in = true;break;
	      }
	    if(in) { // this clause is satisfied
	      for(int k=0;k<j;k++) {
		if(x[k] > 0) LiteralArray[findRefIndex(abs(x[k]))].nP--;
		else if(x[k] < 0) LiteralArray[findRefIndex(abs(x[k]))].nN--;
	      }
	      skipSet[i] = true; // mark clauses as to skip
	      break;
	    }
	  }
	  if(LiteralArray[index].nN > 0) {
	    bool in = false;
	    for(int k=LiteralArray[index].nN-1;k>=0 && LiteralArray[index].negative[k]>=i;k--)
	      if(LiteralArray[index].negative[k] == i) {
		in = true;break;
	      }
	    if(in) x[j] = 0; // eliminate duplicate variable
	    else LiteralArray[index].negative[LiteralArray[index].nN++] = i;
	  } else LiteralArray[index].negative[LiteralArray[index].nN++] = i;
	} 
      }
    }
    for(int j=0;j<3;j++)
      Clauses[i].L[j] = x[j];
  }
}

void printAssign(int ln, int *assign) {
  for(int i=0;i<ln;i++)
    printf("[%lld:%d]", ltrIndex[i], assign[i]);
  puts("");
}

void initLtr(int nClauses) {
  found = false;
  ln = 0;
  map<long long int, int>::iterator it;
  for(int i=0;i<nClauses;i++) {
    if(skipSet[i]) continue;
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

void Divide_Conquer(int nClauses, int depth, int *curAssign) {
  if(found) return;
  
  if(depth == nClauses) {
#pragma omp critical
    {
      for(int i=0;i<ln;i++)
	assignValue[i]=(curAssign[i]==1)?true:false;
      found = true;
    }
    return;
  }

  if(skipSet[depth]) Divide_Conquer(nClauses, depth+1, curAssign);
  else {
    map<long long int, int>::iterator it;
    long long int tmp[3];int vn = 0; // variables in clauses
    int tindex[3]; // variable index
    int asgn[3]; // assigned value
    int remain[3];int rn = 0; // remove prev assigned variables, index = vn
    for(int i=0;i<3;i++)
      if(Clauses[depth].L[i] != 0) {
	tmp[vn] = Clauses[depth].L[i];
	it = ltrMap.find(abs(tmp[vn]));
	tindex[vn] = it->second;
	asgn[vn] = curAssign[it->second];
	if(asgn[vn] == 0) remain[rn++] = vn;
	vn++;
      }

    int possible = 1 << rn;
#pragma omp parallel for
    for(int i=0;i<possible;i++) {
      if (!found) {
	for(int j=0;j<rn;j++) {
	  if(i&(1 << j)) asgn[remain[j]] = 1;
	  else asgn[remain[j]] = -1;
	}
	// check if the assignment is ok
	bool ok = false;
	for(int j=0;j<vn;j++) {
	  if((tmp[j]>0 && asgn[j]==1) || (tmp[j]<0 && asgn[j]==-1)) {
	    ok = true;break;
	  }
	}
	if(ok) {
	  int *newAssign = (int *) malloc(ln*sizeof(int));
	  for(int j=0;j<ln;j++) newAssign[j] = curAssign[j];
	  for(int j=0;j<rn;j++) newAssign[tindex[remain[j]]] = asgn[remain[j]];
	  Divide_Conquer(nClauses, depth+1, newAssign);
	  free(newAssign);
	}
      }
    }
  }
}

int findRefIndex(long long int x) {
  map<long long int, int>::iterator it;
  it = refMap.find(x);
  if(it == refMap.end())
    return -1;
  return it->second;
}

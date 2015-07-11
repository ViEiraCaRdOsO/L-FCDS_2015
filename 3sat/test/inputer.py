###
### Example usage:
###   $ python inputer.py -n 100 -l 5
###
### This produces 100 clauses with 5 literals, sending to stdout.
### Result is always guaranteed to be solvable.
###

from __future__ import print_function
import argparse
import random
import sys

parser = argparse.ArgumentParser(description='Random input generator for 3SAT problem')
parser.add_argument('-n', '--numclauses',
                    required=True,
                    help='Number of clauses')
parser.add_argument('-l', '--numliterals',
                    required=True,
                    help='Number of literals')
parser.add_argument('-s', '--seed',
                    default="0",
                    help='Seed for random generator')

def main():
    args = parser.parse_args()

    N = int(args.numclauses)
    L = int(args.numliterals)
    random.seed(int(args.seed))

    X = random.randint(1, 2**L-1)
    R = str(bin(int(X)))[2:]  # int -> bin
    R = R.zfill(L)                      # add zeroes
    R = R[::-1]                         # original C prog shows bits in reverse
    print("Generating input for solution: %s (%s)" % (X, R), file=sys.stderr)

    print(N, L)
    for i in range(N):
        while True:
            idx1 = random.randint(1, L)
            idx2 = random.randint(1, L)
            idx3 = random.randint(1, L)
            if random.choice([True, False]) == True: idx1 = -idx1
            if random.choice([True, False]) == True: idx2 = -idx2
            if random.choice([True, False]) == True: idx3 = -idx3

            lit1 = '1' if idx1 >= 0 else '0'
            lit2 = '1' if idx2 >= 0 else '0'
            lit3 = '1' if idx3 >= 0 else '0'

            if R[abs(idx1)-1] == lit1 and R[abs(idx2)-1] == lit2 and R[abs(idx3)-1] == lit3:
                # found clause evaluating to True, let's move on
                print(idx1, idx2, idx3)
                break

if __name__ == "__main__":
    main()

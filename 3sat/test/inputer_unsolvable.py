###
### Example usage:
###   $ python inputer.py -n 100 -l 5
###
### This produces 100 random clauses with 5 literals, sending to stdout
### Result is almost always guaranteed to be unsolvable.
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

    print(N, L)
    for i in range(N):
        idx1 = random.randint(1, L)
        idx2 = random.randint(1, L)
        idx3 = random.randint(1, L)
        if random.choice([True, False]) == True: idx1 = -idx1
        if random.choice([True, False]) == True: idx2 = -idx2
        if random.choice([True, False]) == True: idx3 = -idx3
        print(idx1, idx2, idx3)

if __name__ == "__main__":
    main()

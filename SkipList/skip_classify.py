#!/usr/bin/env python

import sys
import rank_skip_list

def _classify(x, c, b):
    if b > 10**-7:
        return (x-c+b)/2/b
    else:
        return 0.5

#@profile
def classify_list(rslist, c, b):
    N = rslist.count()
    zeros = rslist.rank(c-b)
    ones = N-rslist.rank(c+b)

    start = rslist.find_rank(zeros+1) if zeros > 0 else None
    end = rslist.find_rank(N-ones+1) if ones > 0 else None

    output = [_classify(val, c, b) for val in 
              rslist.iterate(start=start, stop=end)]
    return [0]*zeros + output + [1]*ones

def main():

    return 0

if __name__ == "__main__":
    sys.exit(main())

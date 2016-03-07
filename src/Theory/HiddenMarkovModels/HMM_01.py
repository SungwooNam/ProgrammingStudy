# from 'A Revealing Introduction of Hidden Markov Models'

import numpy as NP
import itertools
import functools
import math

pi = NP.array([ 0.6, 0.4])
A = NP.array([ [0.7, 0.3], [0.4, 0.6]])
B = NP.array([ [0.1, 0.4, 0.5], [0.7, 0.2, 0.1] ])
O = NP.array([ 0, 1, 0, 2])

# a-pass
N = B.shape[0]
T = O.shape[0]

a = []
c = []

ap = [ pi[i] * B[i,O[0]] for i in range(0,N) ]
cp = 1.0 / functools.reduce(  (lambda x,y:x+y), ap )
ap = [ api*cp for api in ap ]
a.append( ap )
c.append( cp )

for t in range(1,T) :
    at = []
    for i in range(0,N) :
        ati = functools.reduce( (lambda x,y:x+y), [ ap[j]*A[j,i] for j in range(0,N) ]) * B[i,O[t]]
        at.append(ati)

    ct = 1.0 / functools.reduce(  (lambda x,y:x+y), at )
    at = [ ati*ct for ati in at ]
    a.append( at )
    c.append( ct )
    ap = at

print(a)
print(c)

# b-pass

# r

# Reestimate A, B, pi

# compute P(O|lambda)

# iterate

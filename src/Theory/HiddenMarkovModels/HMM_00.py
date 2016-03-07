# from 'A Revealing Introduction of Hidden Markov Models'

import numpy as NP
import itertools
import functools
import math

pi = NP.array([ 0.6, 0.4])
A = NP.array([ [0.7, 0.3], [0.4, 0.6]])
B = NP.array([ [0.1, 0.4, 0.5], [0.7, 0.2, 0.1] ])
O = NP.array([ 0, 1, 0, 2])


# Brute force

def ProbabilityOfStateSequence( q, pi, A, B, O ):
    p = pi[ q[0] ]*B[ q[0],O[0]]
    #print( 'P(%d) *= %f * %f ' % (0,pi[ state[0] ],B[ state[0],O[0]]) )

    for i in range(1,O.shape[0]):
        qp = q[i-1]
        qi = q[i]
        oi = O[i]
        p *= A[qp,qi] * B[ qi,oi]
        #print( 'P(%d) *= %f * %f ' % (i,A[qp,qi],B[qi,oi]) )

    return p

#Q = NP.array( [1,0,0,0] )
#P_Q = ProbabilityOfStateSequence( Q, pi, A, B, O )
#print('P( %s ) = %f' % (Q,P_Q))

#for Q in itertools.product([0,1],repeat=4):
#    P_Q = ProbabilityOfStateSequence( Q, pi, A, B, O )
#    print('P( %s ) = %f' % (Q,P_Q))


QL = list( itertools.product([0,1],repeat=4) )
P_QL = list( [ ProbabilityOfStateSequence(q, pi, A, B, O) for q in QL] )

max_P_QL = max( P_QL )
index_max_P_QL = P_QL.index(max_P_QL)
print( 'Most probable state sequence is %s with %f' % (QL[index_max_P_QL], max_P_QL ))


# dynamic programming
N = B.shape[0]
T = O.shape[0]
optimal_path = []
Dp = list( map( lambda i : pi[i] * B[i,O[0]], range( 0, N )))
optimal_path.append( Dp.index(max(Dp)) )

for t in range( 1, T ):
    Dt = []

    for i in range( 0, N ) :
        Dtj = list( map(
             lambda j : Dp[j] * A[j,i] * B[i,O[t]],
             range( 0, N )
        ))

        Dt.append( max(Dtj) )
        print( 'Dtj(i=%d) = %s' % (i,Dtj) )

    Dp = Dt
    print( 'Dp(t=%d) = %s' % (t,Dp) )
    optimal_path.append( Dp.index(max(Dp)) )

print( 'dynamic programmming optimal path=%s with %f' % (optimal_path,max(Dp)) )    


#dynamic programming with log - underflow resistent
N = B.shape[0]
T = O.shape[0]
optimal_path = []
Dp = list( map( lambda i : math.log( pi[i] * B[i,O[0]] ), range( 0, N )))
optimal_path.append( Dp.index(max(Dp)) )

for t in range( 1, T ):
    Dt = []

    for i in range( 0, N ) :
        Dtj = list( map(
             lambda j : Dp[j] + math.log(A[j,i]) + math.log(B[i,O[t]]),
             range( 0, N )
        ))

        Dt.append( max(Dtj) )
        print( 'Dtj(i=%d) = %s' % (i,Dtj) )

    Dp = Dt
    print( 'Dp(t=%d) = %s' % (t,Dp) )
    optimal_path.append( Dp.index(max(Dp)) )

print( 'dynamic programmming with log : optimal path=%s with %f' % (optimal_path,max(Dp)) )    

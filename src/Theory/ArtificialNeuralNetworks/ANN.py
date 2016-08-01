# Artificial Neural Network

import numpy as np
import math
import matplotlib.pyplot as plt
import collections

Limit = collections.namedtuple( 'Limit', ['Upper','Lower'])

def Normalize( A, Limits, interval ) :
    N = np.matrix([ (aj-li.Lower)/(li.Upper-li.Lower) 
        for aj,li in zip(A.T, Limits) ])
    N = N * (interval.Upper - interval.Lower) + interval.Lower
    return N.getT() 

def ForwardPropagation( X, HN, ON ) :
    Z = np.dot( HN, X )
    H = [ (1/(1+math.exp(-zi))) for zi in Z ]
    Z = np.dot( ON, H )
    O = [ (1/(1+math.exp(-zi))) for zi in Z ]
    return H, O

def BackwardPropagation( X, HN, ON, H, O, T, nu ) :
    D1 = [ yi*(1-yi)*(ti-yi) for yi,ti in zip(O,T) ]
    D2 = [ hi*(1-hi)*pi for hi, pi in zip(H, np.dot(ON.T,D1))]
    ON += nu * np.dot(np.matrix(D1).T, np.matrix(H))
    HN += nu * np.dot(np.matrix(D2).T, np.matrix(X).T )
    return HN, ON

Examples = np.loadtxt( 'faults.txt' )
Examples = Examples[ range(0,346) ]
A = Examples[:, range(4,27)]

Limits = [Limit(min(aj),max(aj)) for aj in A.T]
A = Normalize( A, Limits, Limit(-1,1))

NE = A.shape[0]
N = A.shape[1]
nu = 0.1

EpochRange = range(0,10)
NA = range( 2, 3)
ErrorHistory = []

for na in NA:
    C = Examples[:, range(27,27+na)]
    M = C.shape[1]
    HN = 0.01 * np.ones( [N,N] )
    ON = 0.01 * np.ones( [M,N] )
 
    ErrorRates = []   
    SumMSEs = []
    for n in EpochRange : 
        sumMiss = 0
        sumMSE = 0
        for Ai,Ci in zip( A,C)  :
             H, O = ForwardPropagation( Ai.T, HN, ON )
             HN, ON = BackwardPropagation( Ai.T, HN, ON, H, O, Ci, nu )             
             miss = 0 if np.argmax(O) == np.argmax(Ci) else 1
             sumMiss += miss
             MSE = sum( [ (y-t)**2 for y,t in zip( O, Ci ) ] )
             sumMSE += MSE
             #print( str.format("miss={0},MSE={1}", miss, MSE))
        
        SumMSEs.append( sumMSE )
        errorRate = sumMiss/NE
        ErrorRates.append( errorRate )
        print( str.format(
            "Epoch {0} has sum of MSE {1} and error rate {2}",
            n, sumMSE, errorRate ))
                          
    ErrorHistory.append( (na,SumMSEs,ErrorRates) )

fig = plt.figure()
'''
sMSE = fig.add_subplot(211)
for na,sumMSEs,errorRates in ErrorHistory:
    sMSE.plot( EpochRange, sumMSEs, label=str.format("{0} attributes", na) )
sMSE.set_ylabel( 'Sum of MSE ( Mean Square Error )')
sMSE.set_xlabel( 'Epoch' )
sMSE.axis( ymin = 0)
sMSE.legend()
'''
sER = fig.add_subplot(111)
for na,sumMSEs,errorRates in ErrorHistory:
    sER.plot( EpochRange, errorRates, label=str.format("{0} attributes", na) )
sER.set_ylabel( 'Error Rate')
sER.set_xlabel( 'Epoch' )
sER.axis( ymin = 0)
sER.legend()
plt.show()            


'''
#HN = np.arange(N*N).reshape((N,N)) * 0.1
#ON = np.arange(M*N).reshape((M,N)) * 0.1
HN = np.array([ [-1, 1], [1,1] ])
ON = np.array([ [1,1], [-1,1] ])
X = np.array([[1,-1]]).T
H, O = ForwardPropagation( X, HN, ON )
HN, ON = BackwardPropagation( X, HN, ON, H, O, np.array([1,0]), 0.1 )
'''

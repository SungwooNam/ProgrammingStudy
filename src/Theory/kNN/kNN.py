# k-Nearest-Neighbors

import numpy as np
import itertools
import functools
import math
from scipy import spatial

def LoadTrainingExamples( file, separator ) :
    '''
>>> LoadTrainingExamples( 'faultsshort.txt', '\t' )
...
    '''    
    examples = []
    for line in open( file ) :
        examples.append( [ float(e) for e in line.rstrip().split(separator) ] )
    return examples


def PickColumns( A, I ) :
    '''
>>> PickColumns( np.array( [[1,2,3],[4,5,6] ] ), [0,2] )
matrix([[1, 3],
        [4, 6]])
    '''
    P = [ A[:,i] for i in I ]
    return np.matrix( P ).getT()


def Normalize( A, minmax ) :
    '''
>>> Normalize( np.matrix('1,2;0,1'), np.matrix('0,0;1,2'))
matrix([[ 1. ,  1. ],
        [ 0. ,  0.5]])
    '''
    N = []
    for i in range(A.shape[1]) :
        minv = minmax[0,i]
        maxv = minmax[1,i]
        N.append( [ ((float(a)-minv)/(maxv-minv)) for a in A[:,i] ] )
    return np.matrix( N ).getT()

def MaxArg( index, C ) :
    if len(index.shape) == 1 :
        S = sum( [ C[i] for i in index ] )
        NS = 1
    else :
        S = sum( [ C[i] for i in index[0] ] )
        NS = len( index[0] )

    return [ int(si/NS+0.5) for si in S ]
    

#Examples = np.array(LoadTrainingExamples( 'faultsshort.txt', '\t' ))
Examples = np.array(LoadTrainingExamples( 'faults.txt', '\t' ))


#AI = range( 0,4 ); CI = range( 4,5 )
AI = range( 4,8 ); CI = range( 27,28 )
error_sum = 0

k = 1
E = PickColumns( Examples, AI );  
minmax = np.vstack( (E.min(0),E.max(0)) ); 
E = Normalize( E, minmax );

EC = PickColumns( Examples, CI )

# LOOCV ( Leave-One-Out Cross Validation )
for out_one in range(len(Examples)): 
    A = np.delete( E, (out_one), axis=0)
    C = np.delete( EC, (out_one), axis=0)
 
    X = E[out_one];
    CX = EC[out_one];

    KD = spatial.KDTree( A )
    distance, index = KD.query( X, k )
    predict = MaxArg( index, C )
    error_sum += sum( [ abs(p-CX[0,i]) for i,p in enumerate(predict)])
    
    print( str.format( "Leave-One {0} is classifed by {1}{2} as {3} while sampled as {4}",
           out_one, index, distance, predict, CX[0] ))  

print( str.format( "Error = {0} ({1}/{2})",
    error_sum/(len(Examples)*len(CI)), error_sum, len(Examples)*len(CI)))  



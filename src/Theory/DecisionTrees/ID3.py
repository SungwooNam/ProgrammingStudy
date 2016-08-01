# from Machine Learning by Tom Mitchell , chapter 3

import numpy as NP
import math

def Entrophy( S ):
    N = sum( S )
    if N == 0.0 : return 0.0

    P = [Si/N for Si in S]
    return sum( [ (-pi * math.log(pi,2) if pi != 0.0 else 0.0) for pi in P] ) 

def Gain( S, A ):
    NS = sum( S )
    NSv = [ sum(Ai) for Ai in A ]
    return Entrophy(S) - sum( [ (NSv[i]/NS)*Entrophy(A[i]) for i in range(len(A))] ) 

def DistinctAttributeMap( O ) :
    DA = []
    for i in range(len(O[0])) :
        m = {}
        A = list(set(O[:,i]))
        for j in range(0,len(A)) : 
             m[ A[j] ] = j
        m[ 'Reverse' ] = A
        m[ 'Count' ] = len(A)
        DA.append( m  )
    return DA

def Attributes( O, DA, i ) : 
    A = NP.zeros( [ DA[i]['Count'], DA[-1]['Count'] ] )

    for j in range(len(O)) :
        a = DA[i][ O[j,i] ]
        c = DA[-1][ O[j,-1] ]
        A[ a, c ] =  A[ a, c ] + 1

    return A

def ID3( Examples, AttributeMap, Availables, Tree  ) :

    C = Attributes( Examples, AttributeMap, -1 )
    S = [ C[i,i] for i in range(len(C)) ]

    print( 'Examples = ', Examples )
    print( 'Availables = ', Availables )
    print( 'S = ', S )

    for i in range(len(S)) :
        if S[i] == len(Examples) :
            Tree[ 'Leaf' ] = AttributeMap[ 'Reverse' ][i]
            return

    if len(Availables) == 0 :
        maxS = max( S )
        maxIndex = S.index( maxS )
        Tree[ 'Probably' ] = AttributeMap[-1][maxIndex]
        Tree[ 'Odd' ] = maxS / sum( S )
        return
    
    maxAttr = -1
    maxGain = -1
    for i in Availables :
        Gi = Gain( S, Attributes( Examples, AttributeMap, i ))
        if Gi > maxGain :
            maxAttr = i
            maxGain = Gi

    Tree[ 'Best' ] = maxAttr
    Tree[ 'Gain' ] = maxGain

    leftAttr = Availables.copy()
    leftAttr.remove( maxAttr )

    for a in AttributeMap[maxAttr][ 'Reverse' ] :
        leftExamples = NP.array( list(filter( lambda r : r[maxAttr] == a, Examples )))

        leftClass = set(leftExamples[:,-1])
        if len( leftClass ) == 1 :
            Tree[ a ] = leftClass.pop()
        else :
            Tree[ a ] = {}  
            ID3( leftExamples, AttributeMap, leftAttr, Tree[a] )
            
    return

O = NP.array([
    ['Sunny', 'Hot', 'High', 'Weak', 'No'],
    ['Sunny', 'Hot', 'High', 'Strong', 'No'],
    ['Overcast', 'Hot', 'High', 'Weak', 'Yes'],
    ['Rain', 'Mild', 'High', 'Weak', 'Yes'],
    ['Rain', 'Cool', 'Normal', 'Weak', 'Yes'],
    ['Rain', 'Cool', 'Normal', 'Strong', 'No'],
    ['Overcast', 'Cool', 'Normal', 'Strong', 'Yes'],
    ['Sunny', 'Mild', 'High', 'Weak', 'No'],
    ['Sunny', 'Cool', 'Normal', 'Weak', 'Yes'],
    ['Rain', 'Mild', 'Normal', 'Weak', 'Yes'],
    ['Sunny', 'Mild', 'Normal', 'Strong', 'Yes'],
    ['Overcast', 'Mild', 'High', 'Strong', 'Yes'],
    ['Overcast', 'Hot', 'Normal', 'Weak', 'Yes'],
    ['Rain', 'Mild', 'High', 'Strong', 'No'],    
    ])

DA = DistinctAttributeMap( O )
DT = {}
ID3( O, DA, [0,1,2,3], DT )
print( 'DecisionTree = ', DT )

''' return
DecisionTree ={
'Best': 0,
'Gain': 0.24674981977443911}
'Rain': {
    'Best': 3,
    'Gain': 0.97095059445466858,
    'Weak': 'Yes',
    'Strong': 'No'},
'Sunny': {
   'Best': 2,
   'Gain': 0.97095059445466858,
   'High': 'No',
   'Normal': 'Yes'},
'Overcast': 'Yes',
'''

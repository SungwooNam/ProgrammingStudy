# REF
# [ML] : Machine Learning, Tom M.Mitchell
# [IML] : An Introduction to Machine Learning, Miroslav Kubat

import numpy as NP
import math
import sys
import collections

def Entrophy( S ):
    N = sum( S ) if len(S) !=0 else sys.float_info.epsilon
    P = [Si/N for Si in S]
    return sum( [ (-pi * math.log(pi,2) if pi != 0.0 else 0.0) for pi in P] ) 

def Gain( S, A ):
    NS = sum( S )
    NSv = [ sum(Ai) for Ai in A ]
    return Entrophy(S) - sum( [ (NSv[i]/NS)*Entrophy(A[i]) for i in range(len(A))] ) 
    
# [IML] P124
# When attribute is continuous, this function finds all the split thresholds 
# and calculate information gain.  
def SplitGains( A, C ):
    Example = collections.namedtuple( 'Example', ['att','label'] )
    E = [ Example(ai,ci) for ai,ci in zip(A,C) ]
    E.sort( key = lambda x : (x.att,x.label) )
    
    L = set( [ ei.label for ei in E ] )
    S = [ sum( [ 1 for ei in E if ei.label == li ] ) for li in L ]
    
    Threshold = collections.namedtuple( 'Threshold', ['att', 'label', 'begin', 'end'])

    Thresholds = []    
    label = E[0].label
    begin = 0
    for (i,ei) in enumerate(E):
        if ei.label == label: continue
        
        middle = (ei.att + E[i-1].att)/2.0 
        Thresholds.append( Threshold(middle, label, begin, i-1))
        
        label = ei.label
        begin = i

    AttGains = []
    for ti in Thresholds:
        left = [ 
            sum( [ 1 for ei in E[:ti.end+1] if ei.label == li ] ) 
            for li in L ]
        right = [ 
            sum( [ 1 for ei in E[ti.end+1:] if ei.label == li ] ) 
            for li in L ]
        AttGains.append( (ti.att, Gain( S, [left,right]) ) )        

    return AttGains        
    

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

import unittest

class TestDecisionTreeID3(unittest.TestCase):

    # [ML] P58
    def test_EntrophySimple(self):
        self.assertAlmostEqual( Entrophy([9,5]), 0.94028595)
        
    def test_MultipleLabel(self):
        print( 'Entrophy([9,5,4])=', Entrophy([9,5,4]) )        
        print( 'Gain([9,5,4],[[6,2,3],[3,3,1]])=', Gain([9,5,4],[[6,2,3],[3,3,1]]) )        
        
    # [ML] P58
    def test_GainSimple(self):
        self.assertAlmostEqual( Gain( [9,5], [[6,2],[3,3]]), 0.04812703)

    def test_EntrophyKnown(self):
        self.assertEqual( Entrophy([100]), 0.0)
        self.assertEqual( Entrophy([]), 0.0)
        self.assertEqual( Entrophy([1,1]), 1.0)
        
    # [IML] P125
    def test_SplitGains(self):
        sg = SplitGains( 
            [ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,11,12,13],
            [ 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0 ])
        self.assertEqual( sg, 
            [ (5.5, 0.49647937549469),
              (10.5, 0.014582259610812609),
              (12.5, 0.09123321517616922) ])
   
    def test_SplitGainsAnyOrder(self):
        sg = SplitGains( 
            [ 6, 7, 8, 9, 10, 1, 2, 3, 4, 5, 11,12,13],
            [ 0, 0, 0, 0, 0,  1, 1, 1, 1, 1, 1, 1, 0 ])
        self.assertEqual( sg, 
            [ (5.5, 0.49647937549469),
              (10.5, 0.014582259610812609),
              (12.5, 0.09123321517616922) ])
        
    def test_SplitGainsSameAtt(self):
        sg = SplitGains( 
            [ 0, 0, 5, 10],
            [ 0, 0, 1, 1 ])
        self.assertEqual( sg, [(2.5, 1.0)])

    def test_SplitGainsSameAttDifferentLabel(self):
        sg = SplitGains( 
            [ 0, 0, 5, 10],
            [ 0, 1, 1, 1 ])
        self.assertEqual( sg, [(0.0, 0.8112781244591328)])
        
    def test_SplitGainsSameAttDifferentLabelMixed(self):
        sg = SplitGains( 
            [ 0, 0, 0, 5, 10],
            [ 0, 1, 0, 1, 1 ])
        self.assertEqual( sg, [(0.0, 0.9709505944546686)])
      
   
if __name__ == '__main__':
    unittest.main()

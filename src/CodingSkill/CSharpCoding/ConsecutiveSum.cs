using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CSharpTest
{
    class ConsecutiveSum
    {
/*
Question:  Given a sequence of positive integers A and an integer T, return whether there is a continuous sequence of A that sums up to exactly T
Example
[23, 5, 4, 7, 2, 11], 20. Return True because 7 + 2 + 11 = 20 
[1, 3, 5, 23, 2], 8. Return True  because 3 + 5 = 8
[1, 3, 5, 23, 2], 7 Return False because no sequence in this array adds up to 7
*/

        bool check_sum( int[] A, int T )
        {
           for( int i=0; i<A.Length; ++i )
           {
              if( recursive_sum( A, i, 0, T ) )
                return true;
           }
   
           return false;
        }

        bool recursive_sum( int[] A, int index, int sum, int T )
        {
            if (index >= A.Length)
                return false;

            int temp = sum + A[index];
            if( temp == T )
                return true;
            else if ( temp > T )
                return false;
     
           return recursive_sum( A, index+1, temp, T );
        }
    }
}

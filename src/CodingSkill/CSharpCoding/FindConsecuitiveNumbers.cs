using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NUnit.Framework;

namespace CSharpTest
{

    public class Range
    {
        public int Start { get; set; }
        public int End { get; set; }
    }

     // [4, 5, 6, 7, 8, 9, 12, 15, 16, 17, 18, 20, 22, 23, 24, 27]

    public class FindConseuitiveNumbers
    {
        // O(n)
        public IEnumerable<Range> Find(IEnumerable<int> values)
        {
            IEnumerator<int> I = values.GetEnumerator();
            if (I.MoveNext() == false)
                yield break;

            Range r = new Range { Start = I.Current, End = I.Current };

            while( I.MoveNext() )
            {
                if (r.End + 1 == I.Current)
                {
                    r.End = I.Current;
                }
                else
                {
                    yield return r;
                    
                    r = new Range { Start = I.Current, End = I.Current };
                }
            }

            yield return r;
        }
    }

    public class FindConseuitiveNumbersInBinarySearch
    {
        // O(log(n))
        public IEnumerable<Range> Find(IEnumerable<int> values)
        {
            int[] A = values.ToArray();

            int lastIndex = 0;
            do
            {
                Range r = new Range { Start = A[lastIndex] };
                lastIndex = BinarySearch(A, lastIndex, lastIndex, A.Length-1);
                r.End = A[lastIndex];
                yield return r;

                lastIndex++;
            } while (lastIndex < A.Length);
        }

        public int BinarySearch(int[] A, int start, int min, int max)
        {
            if (min > max)
            {
                throw new ArgumentException( "Not possible");
            }

            int mid = (int)( (max + min) / 2);

            int diff = A[mid] - (A[start] + (mid-start));
            if (diff > 0)
            {
                return BinarySearch(A, start, min, mid - 1);
            }
            else
            {
                if (diff == 0 && ( (mid >= max )  || (A[mid] + 1 < A[mid + 1]) ) )
                {
                    return mid;
                }

                return BinarySearch(A, start, mid + 1, max);
            }
        }
    }

    [TestFixture]
    public class TestFindConsecutiveNumbers
    {

        [Test]
        public void Test1()
        {
            FindConseuitiveNumbers finder = new FindConseuitiveNumbers();
            Range[] ranges= 
                finder.Find(new int[] { 4, 5, 6, 7, 8, 9, 12, 15, 16, 17, 18, 20, 22, 23, 24, 27 })
                .ToArray();
        }

        [Test]
        public void Test2()
        {
            FindConseuitiveNumbersInBinarySearch finder = new FindConseuitiveNumbersInBinarySearch();
            Range[] ranges =
                finder.Find(new int[] { 4, 5, 6, 7, 8, 9, 12, 15, 16, 17, 18, 20, 22, 23, 24, 27 })
                .ToArray();
        }
    }
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NUnit.Framework;


namespace CSharpTest
{
    // 1101100111 -> convert to 2,2,3 ( consecutive ones ) and 2(1),5(2) ( index of 0 and consecutive count )  
    // sum neighbor and connect index  2,2,3 -> 4[2(1)],5[5(2)] 
    // sort in descending order
    // highest of each consecutive 0 count. 

    public class FlipToMaximizeOnes
    {
        // time complexity : O(n),  space : O(n)
        public void Find( int[] A )
        {
            IList<Token> sorted = GetNeighborSum( Tokenize(A) );

            SortedSet<int> zeroCount = new SortedSet<int>(); // O( log2 n/2 ) worst
            foreach( Token t in sorted )
            {
                if (zeroCount.Contains(t.ZeroCount) == false)
                {
                    Console.WriteLine( "Found {0} zero(s) at {1} which makes {2} ones ", 
                        t.ZeroCount, t.ZeroStartIndex, t.ConsecutiveOnes);

                    zeroCount.Add(t.ZeroCount);
                }
            }
        }

        // time complexity : O( n/2 + n/2 log n/2 ) worst
        IList<Token> GetNeighborSum( IEnumerable<Token> tokens )
        {
            List<Token> summed = new List<Token>();

            IEnumerator<Token> I = tokens.GetEnumerator();
            if (I.MoveNext() == false)
            {
                throw new ArgumentException("None");
            }

            summed.Add(I.Current);
            while (I.MoveNext() )
            {
                summed.Last().ConsecutiveOnes += I.Current.ConsecutiveOnes;
                summed.Add(I.Current);
            }
            summed.Sort( (x,y) => y.ConsecutiveOnes-x.ConsecutiveOnes );
            return summed;
        }

        class Token
        {
            public int ConsecutiveOnes { get; set; }
            public int ZeroStartIndex;
            public int ZeroCount;

            public Token(){}
        }


        // time complexity : O(n)
        IEnumerable<Token> Tokenize(int[] A)
        {
            Token t = new Token();
            for(int i = 0; i<A.Length; ++i )
            {
                if (A[i] == 1)
                {
                    if (t.ZeroCount == 0)
                    {
                        t.ConsecutiveOnes++;
                    }
                    else
                    {
                        yield return t;

                        t = new Token();
                        t.ConsecutiveOnes = 1;
                    }
                }
                else
                {
                    if (t.ZeroCount == 0)
                    {
                        t.ZeroStartIndex = i;
                        t.ZeroCount = 1;
                    }
                    else
                    {
                        t.ZeroCount++;
                    }
                }
            }

            yield return t;
        }

    }

    public class Window
    {
        public int Left { get; set; }
        public int Right { get; set; }
        public int Count { get { return Right - Left; } }
        public int Zeros { get; set; }
    }

    /// <summary>
    ///  other method can be used is sliding window.
    ///  
    ///  1. increase right border and update zero count : only when zero is less than max
    ///  2. increase left border and update zero count  : only when zero is greater than max
    ///  3. update max left-right size
    ///  
    ///  It can be space complexity of O(1) and time complexity of O(n).
    ///  
    ///  though it can't tell how many zeros are the maximum so it should go through all number possible ? 
    /// </summary>
    /// 
    public class MaximizeFlipBySlidingWindow
    {
        // { 1, 1, 0, 1, 1, 0, 0, 1, 1, 0 }
        public Window FindMax(int[] A, int maxZeros)
        {
            Window maxWindow = new Window();
            Window sliding = new Window();

            if (A[sliding.Right] == 0)
            {
                sliding.Zeros++;
            }

            while (sliding.Right < A.Length-1)
            {
                if (sliding.Zeros <= maxZeros)
                {
                    sliding.Right++;
                    if (A[sliding.Right] == 0)
                    {
                        sliding.Zeros++;
                    }
                }

                if (sliding.Zeros > maxZeros)
                {
                    if (A[sliding.Left] == 0)
                    {
                        sliding.Zeros--;
                    }

                    sliding.Left++;
                }

                if (sliding.Zeros == maxZeros)
                {
                    if (sliding.Count > maxWindow.Count)
                    {
                        maxWindow.Left = sliding.Left;
                        maxWindow.Right = sliding.Right;
                        maxWindow.Zeros = sliding.Zeros;
                    }
                }
            }

            return maxWindow;
        }
    }


    [TestFixture]
    public class TestFlip
    {
        [Test]
        public void Test1()
        {
            FlipToMaximizeOnes flipper = new FlipToMaximizeOnes();
            flipper.Find(new int[] { 1, 1, 0, 1, 1, 0, 0, 1, 1, 1 });
        }

        [Test]
        public void Test2()
        {
            MaximizeFlipBySlidingWindow flipper = new MaximizeFlipBySlidingWindow();
            Window w1 = flipper.FindMax(new int[] { 1, 1, 0, 1, 1, 0, 0, 1, 1, 1 }, 1);
            Assert.That(w1.Left, Is.EqualTo(0));
            Assert.That(w1.Right, Is.EqualTo(4));

            Window w2 = flipper.FindMax(new int[] { 1, 1, 0, 1, 1, 0, 0, 1, 1, 1 }, 2);
            Assert.That(w2.Left, Is.EqualTo(3));
            Assert.That(w2.Right, Is.EqualTo(9));
        }

    }
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NUnit.Framework;

namespace CSharpTest
{
    [TestFixture]
    public class FindEvenNumbers
    {
        [Test]
        public void FindEvenNumbers1()
        {
            Assert.AreEqual(4, EvenNumbers(new int[] { 1, 2, 3, 4, 5 }));
            Assert.AreEqual(0, EvenNumbers(new int[] {}));

        }

        int EvenNumbers( int[] A )
        {
            int evens = A.Count(a => a % 2 == 0);
            int odds = A.Length - evens;

            return factSum(evens) + factSum(odds);
        }

        int factSum(int c)
        {
            return (int) ( (c-1) * ( c ) / 2 );
        }
    }
}

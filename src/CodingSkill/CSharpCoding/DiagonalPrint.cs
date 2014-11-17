using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NUnit.Framework;


// 1, 2, 4, 7, 11
// 3, 5, 8, 12,
// 6, 9, 13
// 10, 14,
// 15

// V0j = V0j-1 + j + 2 .  i.e.  6 = 3 + 1+2, 10 = 6 + 2+2
// Vij = vi-1j + i+j+1      i.e.  4 = 2 + 1+1, 7 =  4 + 2+1

namespace CSharpTest
{
    public class DiagonalPrint
    {
        public static IEnumerable<string> Print(int max)
        {
            for (int vj = 1, j = 0; vj <= max; )
            {
                string s = "";
                for (int vi = vj, i = 0; vi <= max; )
                {
                    s += vi + " ";

                    vi = vi + i + j + 1;
                    ++i;
                }

                yield return s;

                vj = vj + j + 2;
                ++j;
            }
        }
    }

    [TestFixture]
    public class TestDiagnoalPrint
    {
        [Test]
        public void TestBasicCase()
        {
            Console.WriteLine("Diagnal Print");
            foreach (string s in DiagonalPrint.Print(16))
            {
                Console.WriteLine(s);
            }
        }
    }
}

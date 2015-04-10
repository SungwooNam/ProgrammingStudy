using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NUnit.Framework;

namespace CSharpTest.GetLongestConsequitiveChars
{
    internal class GetLongestConsequitiveChars
    {
        public IEnumerable<char> Find(string s)
        {
            if (s.Length == 0)
                throw new ArgumentException("should be valid string");

            List<char> maxChars = new List<char>();
            char lastChar = s[0];
            int conseq = 1, maxConseq = 1;

            for (int i = 1; i < s.Length; ++i)
            {
                if (lastChar == s[i])
                {
                    conseq++;

                    if (conseq > maxConseq)
                    {
                        maxConseq = conseq;
                        maxChars.Clear();
                    }
                }
                else
                {
                    if (conseq == maxConseq)
                        maxChars.Add(lastChar);

                    conseq = 1;
                    lastChar = s[i];
                }
            }

            if (conseq == maxConseq)
                maxChars.Add(lastChar);

            return maxChars;
        }
    }

    [TestFixture]
    public class UnitTest
    {
        [Test]
        public void TestBasic()
        {
            var search = new GetLongestConsequitiveChars();

            var ret = search.Find("thiis is a teest");
            Assert.That(ret, Is.EqualTo(new char[] { 'i', 'e' }));

            ret = search.Find("thiiis iss a teessst");
            Assert.That(ret, Is.EqualTo(new char[] { 'i', 's' }));

            ret = search.Find("thiiis iss a teessstttt");
            Assert.That(ret, Is.EqualTo(new char[] { 't'  }));
        }
    }
}

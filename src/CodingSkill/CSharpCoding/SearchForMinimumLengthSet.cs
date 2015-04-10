using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NUnit.Framework;

namespace CSharpTest.SearchForMinimumLengthSet
{
    internal class SearchForMinimumLengthSet
    {
        public string Search(string T, string S)
        {
            Dictionary<char, int> domain = new Dictionary<char, int>();
            foreach (char c in S)
            {
                domain.Add(c, 0);
            }
            int left = 0;
            int right = -1;
            int hit = 0;
            int minLength = int.MaxValue;
            int minLeft = 0;
            int minRight = 0;

            for (; ; )
            {
                if (hit < domain.Count)
                {
                    if (right >= T.Length-1) break;

                    right++;
                    char v = T[right];
                    if (domain.ContainsKey(v) == false) continue;

                    domain[v]++;
                    if (domain[v] == 1)
                        hit++;
                }
                else if (hit == domain.Count)
                {
                    left++;
                    if (left >= T.Length) break;

                    char v = T[left - 1];
                    if (domain.ContainsKey(v) == false) continue;

                    domain[v]--;
                    if (domain[v] == 0) hit--;
                }

                if (hit == domain.Count)
                {
                    int hitLength = right - left + 1;
                    if (hitLength <= minLength)
                    {
                        minLeft = left;
                        minRight = right;
                        minLength = hitLength;
                    }
                }
            }

            if (minLength != int.MaxValue)
            {
                return T.Substring(minLeft, minLength);
            }

            return null;
        }
    }
    
    [TestFixture]
    public class SearchForMinimumLengthSetTest
    {
        [Test]
        public void TestAll()
        {
            var finder = new SearchForMinimumLengthSet();

            var result = finder.Search("abbcbcba", "abc");

            Assert.That(result, Is.EqualTo("cba"));
        }
    }
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NUnit.Framework;

namespace FindAnagram
{
    class AnagramFinder
    {
        class AnagramComparer : IEqualityComparer<string>
        {
            public bool Equals(string x, string y)
            {
                if (x.Length != y.Length)
                {
                    return false;
                }

                return x.All(c => y.Contains(c));
            }

            public int GetHashCode(string obj)
            {
                return obj.Aggregate(0, (total, c) => total + c);
            }
        }

        public void Find(IEnumerable<string> words)
        {
            Dictionary<string, List<string>> m_CategorizedAnagram = 
                new Dictionary<string, List<string>>(
                    new AnagramComparer()); 

            foreach( string w in words ) 
            {
                List<string> samewords = null;
                if( m_CategorizedAnagram.TryGetValue( w, out samewords ) == false )
                {
                    samewords = new List<string>();
                    m_CategorizedAnagram.Add( w, samewords );
                }
                samewords.Add( w ); 
            }

            foreach (KeyValuePair<string, List<string>> kinds in m_CategorizedAnagram)
            {
                foreach (string s in kinds.Value)
                {
                    Console.Write(s + ",");
                }
                Console.Write("\n");
            }
        }
    }

    [TestFixture]
    public class FindAnagramTest
    {
        [Test]
        public void Test1()
        {
            AnagramFinder finder = new AnagramFinder();
            finder.Find( new string[] { "ACT", "TAC", "CAT", "BACC" } );

        }
    }
}


using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NUnit.Framework;

// find all the string and log the position of start 
// then sort in reverse order
// then replace with string index 
// need to log 
// the position should be placed at sored dictionary

// chop out from right. add choped out. then add replacement 

namespace CSharpTest
{
    public class ReplaceString
    {
        class InDescendingOrder : IComparer<int>
        {
            public int Compare(int x, int y)
            {
                return y-x;
            }
        }

        public static string Replace(string[] inputs, string[] outputs, string message)
        {
            // inputs.Length == outputs.Length

            // key is index of string, value is index of input/output
            SortedDictionary<int, int> replaceMap = new SortedDictionary<int, int>( new InDescendingOrder()  ); 

            for( int i=0; i<inputs.Length; ++i)
            {
                for ( int j =0 ; j<message.Length; ) 
                {
                    int foundIndex = message.IndexOf(inputs[i], j);
                    if (foundIndex == -1)
                        break;
                    else
                    {
                        replaceMap.Add(foundIndex, i);

                        j = foundIndex + inputs[i].Length;
                    }
                }
            }

            Stack<string> replaced = new Stack<string>();

            int previous = message.Length;
            foreach (KeyValuePair<int, int> kv in replaceMap)
            {
                int k = kv.Key;
                int v = kv.Value;

                int re = k + inputs[v].Length;

                if (previous - re > 0)
                {
                    replaced.Push(message.Substring(re, previous - re));
                }
                replaced.Push( outputs[v] );

                previous = k;
            }

            StringBuilder builder = new StringBuilder();
            foreach (string s in replaced)
            {
                builder.Append(s);
            }

            return builder.ToString();
        }
    }

    [TestFixture]
    public class TestReplaceString
    {
        [Test]
        public void TestBasicCase()
        {
            string replaced = 
                ReplaceString.Replace(
                    new string[] { "A", "B", "C", "D" },
                    new string[] { "CD", "DD", "EE", "F" },
                    "ABEEECD");
            Assert.AreEqual( replaced, "CDDDEEEEEF" );
        }
    }
}

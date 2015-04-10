using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NUnit.Framework;

namespace CSharpTest
{
    public class BinaryIntegerCode
    {
        class Node
        {
            public string Item { get; set; }

            public Node Left { get; set; }
            public Node Right { get; set; }
        }

        public void DecodePrint(int[] arg)
        {
            RecursivePrint(
                RecurisveBuild(arg, 0, 0)
            );
        }

        public IEnumerable<string> Decode(int[] arg)
        {
            return Enumerate( RecurisveBuild(arg, 0, 0), "" );
        }

        IEnumerable<string> Enumerate(Node n, string code )
        {
            if (n == null)
                yield break;

            if (n.Left == null && n.Right == null )
            {
                yield return code + n.Item;
            }

            foreach (string s in Enumerate(n.Left, code + n.Item).Concat( Enumerate( n.Right, code + n.Item)))
                yield return s;
        }


        void RecursivePrint( Node n )
        {
            if( n == null ) 
                return;

            if( n.Item != null ) 
            {
                Console.Write( n.Item ); Console.Write( "," ); 
            }

            RecursivePrint( n.Left );
            RecursivePrint( n.Right );
        }

       
        Node RecurisveBuild(int[] arg, int i, int count)
        {
            if (i+count > arg.Length)
                return null;

            Node n = new Node { Item = Interpret(arg, i, count) };

            if (n.Item == null)
            {
                return null;
            }

            i += count;

            n.Left = RecurisveBuild( arg, i, 1);
            n.Right = RecurisveBuild( arg, i, 2);
            return n;
        }

        string Interpret(int[] arg, int index, int length)
        {
            if (length < 1 || length > 2 ) return "";
            if (index + length > arg.Length) return "";

            char c = (char)('a' - 1);
            if (length == 1)
            {
                c += (char)arg[index];
            }
            else
            {
                c += (char)( arg[index]*10 + arg[index+1]);
            }

            if (c > 'z')
            {
                return null;
            }

            return new string( new char[] { c });
        }
    }

    [TestFixture]
    public class BinaryIntergerCodeTest
    {
        [Test]
        public void TestAll()
        {
            BinaryIntegerCode decoder = new BinaryIntegerCode();

            string[] ret = decoder.Decode(new int[] { 1, 2, 1 }).ToArray();
            Assert.That(ret, Is.EqualTo(
                new string[] { "aba", "au", "la" }));

            ret = decoder.Decode(new int[] { 1, 1 }).ToArray();
            Assert.That(ret, Is.EqualTo(
                new string[] { "aa", "k" }));

            ret = decoder.Decode(new int[] { 9, 1, 8 }).ToArray();
            Assert.That(ret, Is.EqualTo(
                new string[] { "iah", "ir" }));
        }
    }
}

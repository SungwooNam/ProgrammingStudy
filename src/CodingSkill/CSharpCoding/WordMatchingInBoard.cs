using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NUnit.Framework;

namespace CSharpTest.WordMatchingInBoard
{
    public class Position
    {
        public int x { get; set; }
        public int y { get; set; }
    }

    internal class WordMatchingInBoard
    {
        public SortedDictionary<char, Position> GenerateLookupTable(char[,] board)
        {
            SortedDictionary<char, Position> dic = new SortedDictionary<char, Position>();

            for( int j = 0; j < board.GetLength(0); ++j)
            {
                for (int i = 0; i < board.GetLength(1); ++i)
                {
                    dic.Add( board[j,i], new Position{ x = i, y = j });
                }
            }
            return dic;
        }

        public bool IsValid(string word, SortedDictionary<char,Position> lookup)
        {
            IEnumerator<char> I = word.GetEnumerator();
            if ( !I.MoveNext() )
                return false;

            Position first;
            if (!lookup.TryGetValue(I.Current, out first))
                return false;

            if (!I.MoveNext())
                return true;

            Position second;
            if (!lookup.TryGetValue(I.Current, out second))
                return false;

            int hor = second.x - first.x;
            int ver = second.y - first.y;
            int prev_x = second.x;
            int prev_y = second.y;

            if ( Math.Abs(hor) > 1 || Math.Abs(ver) > 1 )
                return false;

            while (I.MoveNext())
            {
                if (!lookup.TryGetValue(I.Current, out second))
                    return false;

                if (prev_x + hor != second.x ||
                    prev_y + ver != second.y)
                {
                    return false;
                }

                prev_x += hor;
                prev_y += ver;
            }

            return true;
        }
    }


    [TestFixture]
    public class UnitTest
    {
        [Test]
        public void TestBasic()
        {
            var match = new WordMatchingInBoard();
            var lookup = match.GenerateLookupTable(
                new char[4, 4] {  
                    { 'a', 'b', 'c', 'd' },
                    { 'e', 'f', 'g', 'h' },
                    { 'i', 'j', 'k', 'l' },
                    { 'm', 'n', 'o', 'p' } });

            Assert.That(match.IsValid("abcd", lookup), Is.EqualTo(true));
            Assert.That(match.IsValid("g", lookup), Is.EqualTo(true));
            Assert.That(match.IsValid("hg", lookup), Is.EqualTo(true));
            Assert.That(match.IsValid("mjg", lookup), Is.EqualTo(true));
            Assert.That(match.IsValid("jo", lookup), Is.EqualTo(true));

            Assert.That(match.IsValid("q", lookup), Is.EqualTo(false));
            Assert.That(match.IsValid("fh", lookup), Is.EqualTo(false));
            Assert.That(match.IsValid("okc", lookup), Is.EqualTo(false));
            
            Assert.That(match.IsValid("", lookup), Is.EqualTo(false));
        }
    }
}

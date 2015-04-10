using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NUnit.Framework;

namespace CSharpTest.BinarySearchTreeIndentation
{
    internal class Node<T>
    {
        public Node<T> Left { get; set; }
        public Node<T> Right { get; set; }
        public T Data { get; set; }

        public Node( T data, Node<T> left, Node<T> right )
        {
            Data = data;
            Left = left;
            Right = right;
        }
    }

    internal class BinarySearchTreeIndentation
    {
        public int PreTraversal<T>(Node<T> n, int level,
            IDictionary<int, int> cursors,
            IDictionary<Node<T>, int> pos)
        {
            int left;
            if (!cursors.TryGetValue(level, out left))
            {
                left = 0;
                cursors.Add(level, left);
            }
            int right = cursors[level];

            if (n.Left != null)
            {
                left = PreTraversal(n.Left, level + 1, cursors, pos);
            }

            if (n.Right != null)
            {
                right = PreTraversal(n.Right, level + 1, cursors, pos);
            }

            int center = (left + right) / 2;

            pos.Add(n, center);
            cursors[level] = center + 2;
            return center;
        }

        public void VisitRows<T>(Node<T> n, Action<Node<T>> OnVisit, Action OnVisitRowEnd)
        {
            Queue<Node<T>> next = new Queue<Node<T>>();
            Queue<Node<T>> prev = new Queue<Node<T>>();

            next.Enqueue(n);

            while (next.Count != 0)
            {
                Queue<Node<T>> temp = next;
                next = prev;
                prev = temp;

                while (prev.Count != 0)
                {
                    Node<T> p = prev.Dequeue();
                    if (p.Left != null) next.Enqueue(p.Left);
                    if (p.Right != null) next.Enqueue(p.Right);

                    OnVisit(p);
                }

                OnVisitRowEnd();
            }
        }

        public void PrintIndentedTree<T>(Node<T> tree)
        {
            IDictionary<int,int> cursors = new Dictionary<int,int>();
            IDictionary<Node<T>,int> pos = new Dictionary<Node<T>,int>();
            PreTraversal(tree, 0, cursors, pos);

            Console.Write("\n");
            int cursor = 0;
            VisitRows(tree,
                (n) =>
                {
                    while( cursor++ < pos[n] )  
                        Console.Write(" ");

                    Console.Write(n.Data);
                },
                () => {
                    cursor = 0;
                    Console.Write("\n");
                });
        }

        public int Traverse<T>(Node<T> n, int level, IDictionary<int, StringBuilder> lines)
        {
            if (n == null)
            {
                StringBuilder line;
                if (lines.TryGetValue(level - 1, out line) == false)
                {
                    lines.Add(level - 1, line = new StringBuilder());
                }

                return line.Length;
            }

            int left = Traverse(n.Left, level + 1, lines);
            int right = Traverse(n.Right, level + 1, lines);
            int center = (left + right) / 2;

            StringBuilder row;
            if (lines.TryGetValue(level, out row) == false)
            {
                lines.Add(level, row = new StringBuilder());
            }
            int spaces = Math.Max(0,center - row.Length-1);
            row.Append(' ', spaces);
            row.Append(n.Data);
            row.Append(' ', 1);

            return row.Length;
        }

        public void PrintIndentedTreeByTraverse<T>(Node<T> tree)
        {
            var lines = new Dictionary<int, StringBuilder>();
            Traverse(tree, 0, lines);

            Console.Write("\n");
            lines.OrderBy( kv => kv.Key).Count((kv) =>
            {
                Console.WriteLine(kv.Value.ToString());
                return true;
            });
        }

    }

    [TestFixture]
    public class UnitTest
    {
        [Test]
        public void TestBasic()
        {
            Node<char> tree = 
                new Node<char>( 'a', 
                    new Node<char>( 'b', 
                        new Node<char>( 'd', null, null ),
                        new Node<char>( 'e', null, null ) ),
                    new Node<char>( 'c', 
                        new Node<char>( 'f', null, null ),
                        new Node<char>( 'g', null, null ) ));


            var printer = new BinarySearchTreeIndentation();
            printer.PrintIndentedTree( tree );
        }

        [Test]
        public void TestLeftSide()
        {
            Node<char> tree =
                new Node<char>('a',
                    new Node<char>('b',
                        new Node<char>('d', null, null),
                        new Node<char>('e', null, null)),
                    new Node<char>('c', null, null ));


            var printer = new BinarySearchTreeIndentation();
            printer.PrintIndentedTree(tree);
        }

        [Test]
        public void TestRightSide()
        {
            Node<char> tree =
                new Node<char>('a',
                    new Node<char>('b', null, null ), 
                    new Node<char>('c',
                        new Node<char>('f', null, null),
                        new Node<char>('g', null, null)));


            var printer = new BinarySearchTreeIndentation();
            printer.PrintIndentedTree(tree);
        }

        [Test]
        public void TestTraverse()
        {
            var printer = new BinarySearchTreeIndentation();

            {
                Node<char> tree =
                    new Node<char>('a',
                        new Node<char>('b',
                            new Node<char>('d', null, null),
                            new Node<char>('e', null, null)),
                        new Node<char>('c',
                            new Node<char>('f', null, null),
                            new Node<char>('g', null, null)));

                printer.PrintIndentedTreeByTraverse(tree);
            }

            {
                Node<char> tree =
                    new Node<char>('a',
                        new Node<char>('b',
                            new Node<char>('d', null, null),
                            new Node<char>('e', null, null)),
                        new Node<char>('c', null, null));

                printer.PrintIndentedTreeByTraverse(tree);
            }

            {
                Node<char> tree =
                    new Node<char>('a',
                        new Node<char>('b', null, null),
                        new Node<char>('c',
                            new Node<char>('f', null, null),
                            new Node<char>('g', null, null)));

                printer.PrintIndentedTreeByTraverse(tree);
            }
        }
    }
}

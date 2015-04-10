using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NUnit.Framework;

namespace CSharpTest.BinaryTreeVisitRow
{
    internal class Node<T>
    {
        public Node<T> Left { get; set; }
        public Node<T> Right { get; set; }
        public T Data { get; set; }

        public static Node<T> CreateNode<T>(T data, Node<T> left, Node<T> right)
        {
            return new Node<T>
            {
                Data = data,
                Left = left,
                Right = right
            };
        }
    }


    internal class BinaryTreeVisitRow
    {
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

        public void PrintRowByRow<T>(Node<T> n)
        {
            VisitRows( 
                n, 
                (node) => Console.Write( node.Data.ToString()), 
                () => Console.Write( "\n" ) );
        }
    }

    [TestFixture]
    public class UnitTest
    {
        [Test]
        public void TestPrint()
        {
            Node<char> t = Node<char>
                .CreateNode('a',
                    Node<char>.CreateNode('b',
                        Node<char>.CreateNode('d', null, null),
                        Node<char>.CreateNode('e', null, null)),
                    Node<char>.CreateNode('c',
                        Node<char>.CreateNode('f', null, null),
                        Node<char>.CreateNode('g', null, null)));



            BinaryTreeVisitRow visitor = new BinaryTreeVisitRow();
            visitor.PrintRowByRow(t);
        }
    }
}

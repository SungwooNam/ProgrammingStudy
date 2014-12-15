using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NUnit.Framework;

namespace CSharpTest
{
    //
    // http://www.cs.cmu.edu/~adamchik/15-121/lectures/Trees/trees.html
    //
    public enum TraverseOrder
    {
        PreOrder = 0,
        InOrder,
        PostOrder,
        Level,
    }

    public class BinarySearchTree<T>
    {
        internal class Node
        {
            public Node( T k ) { Key = k; }

            public T Key { get; set; }

            public Node Left { get; set; }
            public Node Right { get; set; }
        };

        IComparer<T> Compare { get; set; }
        Node Root { get; set; }

        public BinarySearchTree(IComparer<T> compare)
        {
            Compare = compare;
        }

        class FuncComparer : IComparer<T>
        {
            public Func<T, T, int> FC { get; set; }

            public int Compare(T x, T y)
            {
                return FC.Invoke(x, y);
            }
        }

        public BinarySearchTree(IEnumerable<T> items, Func<T,T,int> compare)
        {
            Compare = new FuncComparer { FC = compare };

            foreach (T k in items)
            {
                Add(k);
            }
        }

        public BinarySearchTree(IEnumerable<T> items, IComparer<T> compare)
        {
            Compare = compare;

            foreach (T k in items)
            {
                Add(k);
            }
        }

        public void Add(T key )
        {
            if (Root == null)
            {
                Root = new Node(key);
                return;
            }
            else
            {
                RecursiveAdd(key, Root);
            }
        }

        void RecursiveAdd(T key, Node at)
        {
            int result = Compare.Compare( key, at.Key);
            if (result < 0)
            {
                if (at.Left == null)
                {
                    at.Left = new Node(key);
                    return;
                }
                else
                {
                    RecursiveAdd(key, at.Left);
                }
            }
            else if ( result > 0 )
            {
                if (at.Right == null)
                {
                    at.Right = new Node(key);
                    return;
                }
                else
                {
                    RecursiveAdd(key, at.Right);
                }
            }
            else
            {
                throw new ArgumentException( string.Format( 
                    "{0} already exist", key ));
            }
        }
        
        delegate IEnumerable<T> TraversalWay( Node n );


        static IDictionary<TraverseOrder, TraversalWay> _TraversalSupported;
        static BinarySearchTree()
        {
            _TraversalSupported = new Dictionary<TraverseOrder, TraversalWay>();
            _TraversalSupported.Add(TraverseOrder.PreOrder, PreOrder );
            _TraversalSupported.Add(TraverseOrder.PostOrder, PostOrder);
            _TraversalSupported.Add(TraverseOrder.InOrder, InOrder);
        }

        public IEnumerable<T> Traverse( TraverseOrder order)
        {
            TraversalWay traversal = _TraversalSupported[order];
            foreach (T k in traversal(Root))
            {
                yield return k;
            }
        }

        static IEnumerable<T> PreOrder( Node n )
        {
            if( n == null )
                yield break;

            yield return n.Key;

            if (n.Left != null)
            {
                foreach (T k in PreOrder(n.Left))
                    yield return k;
            }

            if (n.Right != null)
            {
                foreach (T k in PreOrder(n.Right))
                    yield return k;
            }
        }

        static IEnumerable<T> PostOrder(Node n)
        {
            if (n == null)
                yield break;

            if (n.Left != null)
            {
                foreach (T k in PostOrder(n.Left))
                    yield return k;
            }

            if (n.Right != null)
            {
                foreach (T k in PostOrder(n.Right))
                    yield return k;
            }

            yield return n.Key;
        }

        static IEnumerable<T> InOrder(Node n)
        {
            if (n == null)
                yield break;

            if (n.Left != null)
            {
                foreach (T k in InOrder(n.Left))
                    yield return k;
            }

            yield return n.Key;

            if (n.Right != null)
            {
                foreach (T k in InOrder(n.Right))
                    yield return k;
            }
        }

        public void Restore(T[] PreOrder, T[] InOrder)
        {
            Root = Restore(PreOrder, 0, PreOrder.Length - 1, InOrder, 0, InOrder.Length - 1);
        }

        Node Restore(T[] P, int PS, int PE, T[] I, int IS, int IE )
        {
            if (PS <= PE)
            {
                int C = 0;
                for (; Compare.Compare(I[IS+C], P[PS]) != 0; C++) ;

                Node n = new Node(P[PS]);
                n.Left = Restore(P, PS + 1, PS + C, I, IS, IS + C - 1);
                n.Right = Restore(P, PS + C + 1, PE, I, IS + C + 1, IE);
                return n;
            }
            return null;
        }
    }


    [TestFixture]
    public class BinarySearchTreeTest
    {
        [Test]
        public void TestConstructor()
        {
            BinarySearchTree<int> BST = new BinarySearchTree<int>(
                new int[] { 8, 3, 9, 1, 5, 12, 4, 11 },
                ( (x, y) => x - y )
            );

            Assert.That(
                BST.Traverse(TraverseOrder.PreOrder).ToArray(),
                Is.EqualTo(new int[] { 8, 3, 1, 5, 4, 9, 12, 11 }));
            Assert.That(
                BST.Traverse(TraverseOrder.PostOrder).ToArray(),
                Is.EqualTo(new int[] { 1, 4, 5, 3, 11, 12, 9, 8 }));
            Assert.That(
                BST.Traverse(TraverseOrder.InOrder).ToArray(),
                Is.EqualTo(new int[] { 1, 3, 4, 5, 8, 9, 11, 12 }));
        }

        [Test]
        public void TestRestore()
        {
            BinarySearchTree<int> BST = 
                new BinarySearchTree<int>( Enumerable.Empty<int>(), (x, y) => x - y);

            BST.Restore(
                new int[] { 8, 3, 1, 5, 4, 9, 12, 11 }, 
                new int[] { 1, 3, 4, 5, 8, 9, 11, 12 });

            Assert.That(
                BST.Traverse(TraverseOrder.PostOrder).ToArray(),
                Is.EqualTo(new int[] { 1, 4, 5, 3, 11, 12, 9, 8 }));
        }
    }
}


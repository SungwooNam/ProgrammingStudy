using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NUnit.Framework;

namespace CSharpTest.BTreeToList
{
    internal class BTreeNode<T>
    {
        public T Item { get; set; }
        public BTreeNode<T> Left { get; set; }
        public BTreeNode<T> Right { get; set; }
    }

    internal class LinkedNode<T>
    {
        public T Item { get; set; }
        public LinkedNode<T> Next { get; set; }
        public LinkedNode<T> Prev { get; set; }
    }

    internal delegate void OnVisit<TItem,TArg>( BTreeNode<TItem> bnode, ref TArg arg );

    internal class BinaryTreeToDoublyLinkedListInOrder
    {
        public static BTreeNode<T> CreateBTree<T>( IEnumerable<T> array, Func<T,T,int> comparer)
        {
            BTreeNode<T> node = new BTreeNode<T>();
            node.Item = array.First();

            foreach( T item in array.Skip(1) )
            {
                AddBTree( node, item, comparer );
            }

            return node;
        }


        public static void AddBTree<T>( BTreeNode<T> node, T item, Func<T,T,int> comparer )
        {
            if( comparer.Invoke( item, node.Item ) < 0 )
            {
                if( node.Left == null ) 
                {
                    node.Left = new BTreeNode<T>();
                    node.Left.Item = item;
                    return;
                }
                AddBTree<T>( node.Left, item, comparer );
            }
            else
            {
                if( node.Right == null ) 
                {
                    node.Right = new BTreeNode<T>();
                    node.Right.Item = item;
                    return;
                }
                AddBTree<T>( node.Right, item, comparer );
            }
        }

        public static void TraverInOrder<TItem, TArg>( 
            BTreeNode<TItem> bnode, OnVisit<TItem, TArg> onVisit, ref TArg arg)
        {
            if (bnode == null) return;

            TraverInOrder(bnode.Left, onVisit, ref arg);
            onVisit.Invoke(bnode, ref arg);
            TraverInOrder(bnode.Right, onVisit, ref arg);
        }

        public static void BuildDoublyLinkedList<TItem>(
            BTreeNode<TItem> bnode, ref LinkedNode<TItem> prev)
        {
            LinkedNode<TItem> next = new LinkedNode<TItem>();
            next.Item = bnode.Item;
            prev.Next = next;
            next.Prev = prev;
            prev = next;
        }

        public static IEnumerable<T> EnumerateLinkedList<T>(LinkedNode<T> head)
        {
            for( head = head.Next; head != null; head= head.Next )
            {
                yield return head.Item;
            }
        }
    }
   
    [TestFixture]
    public class BinaryTreeToDoublyLinkedListInOrderTest
    {
        [Test]
        public void TestAll()
        {
            BTreeNode<int> btree = 
                BinaryTreeToDoublyLinkedListInOrder.CreateBTree( 
                    new int[] { 5, 3, 8, 1, 4 } ,  
                    (x,y) => x - y );

            LinkedNode<int> head = new LinkedNode<int>();
            LinkedNode<int> tail = head;
            BinaryTreeToDoublyLinkedListInOrder.TraverInOrder(btree,
                BinaryTreeToDoublyLinkedListInOrder.BuildDoublyLinkedList<int>,
                ref tail);
            var result =
                BinaryTreeToDoublyLinkedListInOrder.EnumerateLinkedList(head).ToArray();

            Assert.That(result, Is.EqualTo(
                new int[] { 1, 3, 4, 5, 8 }));
        }
    }
}

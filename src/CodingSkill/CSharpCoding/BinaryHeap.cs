using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NUnit.Framework;

namespace CSharpTest
{
    //
    // http://en.wikipedia.org/wiki/Binary_heap
    // 

    public class IntegerBinaryHeap
    {
        internal int[] Heap;
        internal int HeapLength;

        public int Length { get { return HeapLength; } }
        public IEnumerable<int> Values
        {
            get
            {
                return Heap.Take(HeapLength);
            }
        }
       
        // O(n)
        public IntegerBinaryHeap( IEnumerable<int> heap )
        {
            HeapLength = heap.Count();
            int binarySize = (int)Math.Pow(2, Math.Floor(Math.Log(HeapLength + 1,2))+1);

            Heap = new int[binarySize];
            IEnumerator<int> I = heap.GetEnumerator();
            for (int i = 0;  ; ++i )
            {
                if( I.MoveNext() == false )
                    break;

                Heap[i] = I.Current;
            }

            for (int i = (int)(HeapLength / 2)-1; i >= 0; --i)
            {
                MaxHeapify( i);
            }
        }

        // O(logn)
        void MaxHeapify( int i)
        {
            int left = i * 2 + 1;
            int right = left + 1;
            int largest = i;

            if (left <= HeapLength - 1 && Heap[left] > Heap[largest])
                largest = left;

            if (right <= HeapLength - 1 && Heap[right] > Heap[largest])
                largest = right;

            if (largest != i)
            {
                int temp = Heap[i];
                Heap[i] = Heap[largest];
                Heap[largest] = temp;
                MaxHeapify( largest);
            }
        }

        public void Insert( int v)
        {
            if (HeapLength == Heap.Length)
            {
                int binarySize = (int)Math.Pow(2, Math.Floor(Math.Log(HeapLength + 1))+1 );

                int[] NewHeap = new int[binarySize];
                for (int i = 0; i<Heap.Length; ++i)
                {
                    NewHeap[i] = Heap[i];
                }

                Heap = NewHeap;
            }

            Heap[HeapLength] = v;
            HeapLength++;

            UpHeap(HeapLength - 1);
        }

        void UpHeap(int i)
        {
            int parent = (i-1) / 2;
            if (parent >= 0 && Heap[parent] < Heap[i])
            {
                int temp = Heap[parent];
                Heap[parent] = Heap[i];
                Heap[i] = temp;
                UpHeap( parent );
            }
        }

        public int Pop()
        {
            int top = Heap[0];

            Heap[0] = Heap[HeapLength-1];
            HeapLength--;
            MaxHeapify(0);

            return top;
        }
    }

    [TestFixture]
    public class BinaryHeapTest
    {
        [Test]
        public void ShouldGenerateHeapifiedHeap()
        {
            IntegerBinaryHeap bheap = new IntegerBinaryHeap( new int[] { 1, 2, 3, 4, 5, 6 } );
            Assert.That(bheap.Values.ToArray(), Is.EqualTo(new int[] { 6, 5, 3, 4, 2, 1 }));
        }

        [Test]
        public void ShouldInsertKeepHeapOrder()
        {
            IntegerBinaryHeap bheap = new IntegerBinaryHeap(new int[] { 1, 2, 3, 4, 5, 6 });
            bheap.Insert(7);
            Assert.That(bheap.Values.ToArray(), Is.EqualTo(new int[] { 7, 5, 6, 4, 2, 1, 3 }));
        }

        [Test]
        public void ShouldPopTopAndKeepHeapOrder()
        {
            IntegerBinaryHeap bheap = new IntegerBinaryHeap(new int[] { 1, 2, 3, 4, 5, 6 });
            int top = bheap.Pop();
            Assert.That(top, Is.EqualTo(6));
            Assert.That(bheap.Values.ToArray(), Is.EqualTo(new int[] { 5, 4, 3, 1, 2 }));
        }
    }


}

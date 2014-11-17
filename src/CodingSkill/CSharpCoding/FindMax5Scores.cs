using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NUnit.Framework;

namespace AMZTest
{

    public struct Student 
    {
        public int score;
        public int student_id;
        public DateTime date;
    }

    public class StudentScoreBoard
    {
        class ScoreComparer : IComparer<int>
        {
            public int Compare( int x, int y)
            {
                return y - x;
            }
        }

        ScoreComparer _ScoreComparer = new ScoreComparer();

        // O( n ) in process time and memory space
        public void Process(IEnumerable<Student> results)
        {
            Dictionary<int, List<int>> boards = new Dictionary<int, List<int>>();
            foreach (Student s in results)
            {
                List<int> scores = null;
                if (boards.TryGetValue(s.student_id, out scores) == false)
                {
                    scores = new List<int>();
                    boards.Add(s.student_id, scores);
                }

                scores.Add(s.score);
            }  // O( n ) here

            foreach (KeyValuePair<int, List<int>> b in boards)
            {
                int student_id = b.Key;
                List<int> scores = b.Value;

                scores.Sort( _ScoreComparer );  // O( n log(n) ) here
                int top5 = Math.Min(5, scores.Count);
                int sum = 
                    scores
                        .Take(top5)
                        .Aggregate(0, (total, s) => total + s );
                double averageOfTop5 = (double)sum / top5;
            }
        }
    }

    [TestFixture]
    public class FindMax5Scores
    {
        [Test]
        public void Test1()
        {
            StudentScoreBoard board = new StudentScoreBoard();

            List<Student> students = new List<Student>();
            students.Add( new Student{ student_id = 1, score = 98 } );
            students.Add( new Student{ student_id = 1, score = 93 } );
            students.Add( new Student{ student_id = 1, score = 99 } );
            students.Add( new Student{ student_id = 1, score = 99 } );
            students.Add( new Student{ student_id = 1, score = 99 } );
            students.Add( new Student{ student_id = 1, score = 99 } );
            students.Add( new Student{ student_id = 1, score = 99 } );

            board.Process(students);

        }

    }
}

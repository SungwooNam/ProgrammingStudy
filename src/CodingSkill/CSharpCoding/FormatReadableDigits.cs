using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NUnit.Framework;


/*
This test was on https://www.hackerrank.com 

Input is a string of Bytes E.g.341B 
Convert it to human readable form: 3 characters long (excluding decimal) 
No trailing or leading zeros 
E.g: 
Input 341B 
Output 341B 
Input 12345B 
Output 12.3K 
Input 1234567B 
Output 1.23M 
Input 1000000000B 
Output 1G 

Do not round off 
Assume input will not be more than 1G 
For this problem 1000B = 1K, so on and so forth

- JSDUDE on October 22, 2014 in United States for TRMS Report Duplicate | Flag 
*/

namespace CSharpTest
{
    public class FormatReadableDigits
    {
        static string[] Suffix = new string[] { "B", "K", "M", "G" };

        public string Fomat(string B)
        {
            int N = B.Length - 1;
            int M = (N-1) / 3;
            string SU = Suffix[M];

            int F = M*3;
            int DC = N - F;
            int FSC = Math.Max( 0, Math.Min( 3, B.Length) - DC);
            string FSCS = B.Substring( DC, FSC );
            if( FSCS.All( c => c == '0' ) )
                FSCS = "";

            string DOT = ".";
            if( FSCS.Length == 0 )
                DOT = "";

            string ret = string.Format("{0}{1}{2}{3}",
                B.Substring(0, DC),
                DOT,
                FSCS,
                SU);

            return ret;
        }
    }

    [TestFixture]
    public class FormatTest
    {
        [Test]
        public void Test1()
        {
            FormatReadableDigits formatter = new FormatReadableDigits();

            string r = "";
            
            r = formatter.Fomat("123B");
            r = formatter.Fomat("12345B");
            r = formatter.Fomat("1234567B");
            r = formatter.Fomat("1000000000B");
        }
    }
}

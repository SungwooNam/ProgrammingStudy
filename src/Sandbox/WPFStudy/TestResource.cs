using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NUnit.Framework;
using System.Windows;
using System.Threading;

namespace WPFStudy
{
    [TestFixture]
    public class TestResource
    {
        [Test]
        public void TestResourceWindow()
        {
            using (WindowPlayer runner = new WindowPlayer())
            {
                Window window = null;

                runner.Start(delegate
                {
                    window = new TestResourceWindow();
                    window.Show();
                    return window;
                });

                runner.Invoke(delegate
                {

                });

                Thread.Sleep(10000);
            }
        }
    }
}

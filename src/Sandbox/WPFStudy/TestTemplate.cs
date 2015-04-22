using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NUnit.Framework;
using System.Windows;
using System.Threading;

// xaml example & code from "WPF4 Unleashed" by Adam Nathan

namespace WPFStudy
{
    [TestFixture]
    public class FixtureTestTemplate
    {
        [Test]
        public void TestControlTemplate()
        {
            using (WindowPlayer runner = new WindowPlayer())
            {
                Window window = null;

                runner.Start(delegate
                {
                    window = new TestTemplate.UsingControlTemplate();
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

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NUnit.Framework;
using System.Windows;
using System.Windows.Controls;
using System.Threading;

namespace WPFStudy
{
    [TestFixture]
    public class TestHelloWorld
    {
        [Test]
        public void TestSimpleHelloWorld()
        {
            using (WindowPlayer runner = new WindowPlayer())
            {
                Window window = null;

                runner.Start(delegate
                {
                    window = new Window();
                    window.Title = "Hello World";
                    window.Show();
                    return window;
                });

                runner.Invoke(delegate
                {
                    Assert.AreEqual(true, window.IsVisible);
                    Assert.Greater(window.ActualWidth, 0);
                    Assert.Greater(window.ActualHeight, 0);
                    Assert.AreEqual("Hello World", window.Title);
                });

                Assert.AreEqual("Hello World", runner.Invoke(delegate { return window.Title; }));
            }

        }

        [Test]
        public void TestHelloWorldWithAButton()
        {
            using (WindowPlayer runner = new WindowPlayer())
            {
                Window window = null;

                runner.Start(delegate
                {
                    window = new Window();
                    window.Title = "Hello World";
                    window.Show();

                    window.Content = new Button();
                    return window;
                });


                runner.Invoke(delegate
                {
                    FrameworkElement element = window.Content as FrameworkElement;
                    Window parent = element.Parent as Window;
                    Assert.AreEqual("Hello World", parent.Title);
                });
            }
        }
    }
}

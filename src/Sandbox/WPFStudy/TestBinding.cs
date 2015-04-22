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
    public class FxitureTestBinding
    {
        [Test]
        public void TestBinding_UsingBinding()
        {
            using (WindowPlayer runner = new WindowPlayer())
            {
                Window window = null;

                runner.Start(delegate
                {
                    window = new TestBinding.UsingBinding();
                    window.Show();
                    return window;
                });

                runner.Invoke(delegate
                {

                });

                // Thread.Sleep(10000);
            }
        }


        [Test]
        public void TestBinding_UsingObservableCollection()
        {
            using (WindowPlayer runner = new WindowPlayer())
            {
                Window window = null;

                runner.Start(delegate
                {
                    window = new TestBinding.UsingObservableCollection();
                    window.Show();
                    return window;
                });

                runner.Invoke(delegate
                {

                });

                //Thread.Sleep(10000);
            }
        }

        [Test]
        public void TestBinding_UsingDataTemplate()
        {
            using (WindowPlayer runner = new WindowPlayer())
            {
                Window window = null;

                runner.Start(delegate
                {
                    window = new TestBinding.UsingDataTemplate();
                    window.Show();
                    return window;
                });

                runner.Invoke(delegate
                {

                });

                //Thread.Sleep(10000);
            }
        }

        [Test]
        public void TestBinding_UsingValueConverter()
        {
            using (WindowPlayer runner = new WindowPlayer())
            {
                Window window = null;

                runner.Start(delegate
                {
                    window = new TestBinding.UsingConversion();
                    window.Show();
                    return window;
                });

                runner.Invoke(delegate
                {

                });

                //Thread.Sleep(10000);
            }
        }
    }
}

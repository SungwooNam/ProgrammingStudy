using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Media;
using System.Threading;
using System.Windows.Threading;

namespace WPFStudy
{
    public class WindowPlayer : IDisposable
    {
        private Thread _Thread;
        private Window _Window;

        public WindowPlayer()
            : this(null)
        {
        }

        public WindowPlayer(Func<Window> originalDeleagate)
        {
            _Thread = null;

            if (originalDeleagate != null)
            {
                Start(originalDeleagate);
            }
        }

        public void Start(Func<Window> originalDeleagate)
        {
            AutoResetEvent started = new AutoResetEvent(false);

            _Thread = new Thread(
                delegate()
                {
                    _Window = originalDeleagate.Invoke();
                    started.Set();

                    System.Windows.Threading.Dispatcher.Run();
                });

            _Thread.SetApartmentState(ApartmentState.STA);

            _Thread.Start();

            started.WaitOne();
        }

        public void Invoke(Action action)
        {
            _Window.Dispatcher.Invoke(action);
        }

        public TResult Invoke<TResult>(Func<TResult> action)
        {
            return (TResult)_Window.Dispatcher.Invoke(action);
        }

        private void Exit()
        {
            if (_Thread.IsAlive)
            {
                _Window.Dispatcher.InvokeShutdown();
            }

            _Thread.Join();
            _Thread = null;
        }

        #region IDisposable Members

        ~WindowPlayer()
        {
            Dispose(false);
        }

        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        protected virtual void Dispose(bool disposing)
        {
            if (disposing)
            {
                if (_Thread != null)
                {
                    Exit();
                }
            }
        }

        #endregion
    }

}


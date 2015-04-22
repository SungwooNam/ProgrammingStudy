using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Collections.ObjectModel;

namespace WPFStudy.TestBinding
{
    /// <summary>
    /// Interaction logic for TestBinding.xaml
    /// </summary>
    public partial class UsingDataTemplate : Window
    {
        public UsingDataTemplate()
        {
            InitializeComponent();

            Photos photos = (Photos)Resources["photos"];
            photos.Add( new Photo( ".\\Resources\\bird.gif" ) );
        }

        private void OnCheckClick(object sender, RoutedEventArgs e)
        {
        }
    }
}

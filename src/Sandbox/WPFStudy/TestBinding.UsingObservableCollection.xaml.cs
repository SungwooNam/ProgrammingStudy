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
    public partial class UsingObservableCollection : Window
    {
        public UsingObservableCollection()
        {
            InitializeComponent();
        }

        private void AddClicked(object sender, RoutedEventArgs e)
        {
            ((Photos)Resources[ "photos" ]).Add(new Photo( "Photo Path"));
        }

        private void AddAlbumClicked(object sender, RoutedEventArgs e)
        {
            ((Photos)Resources["album"]).Add(new Photo("Photo Path"));
        }
    }
}

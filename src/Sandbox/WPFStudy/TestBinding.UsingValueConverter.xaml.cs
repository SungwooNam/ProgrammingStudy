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
using System.Globalization;

namespace WPFStudy.TestBinding
{
    /// <summary>
    /// Interaction logic for TestBinding.xaml
    /// </summary>
    public partial class UsingConversion : Window
    {
        public UsingConversion()
        {
            InitializeComponent();
        }

        private void OnAdd(object sender, RoutedEventArgs e)
        {
            ((Photos)Resources["photos"]).Add(new Photo(".\\Resources\\bird.gif"));
        }
    }

    public class CountToBackgroundConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            if (targetType != typeof(Brush))
            throw new InvalidOperationException("The target must be a Brush!");
            // Let Parse throw an exception if the input is bad
            int num = int.Parse(value.ToString());
            return (num == 0 ? Brushes.Yellow : Brushes.Transparent);
        }
        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return DependencyProperty.UnsetValue;
        }
    }
}

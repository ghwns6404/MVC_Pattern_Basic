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

namespace GradientRainbow
{
    /// <summary>
    /// MainWindow.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            Title = "Follow the Rainbow";

            LinearGradientBrush brush = new LinearGradientBrush();

            brush.StartPoint = new Point(0, 0);
            brush.EndPoint = new Point(1, 0);

            brush.GradientStops.Add(new GradientStop(Colors.Red, 0));
            brush.GradientStops.Add(new GradientStop(Colors.Orange, 0.17));
            brush.GradientStops.Add(new GradientStop(Colors.Yellow, 0.33));
            brush.GradientStops.Add(new GradientStop(Colors.Green, 0.5));
            brush.GradientStops.Add(new GradientStop(Colors.Blue, 67));
            brush.GradientStops.Add(new GradientStop(Colors.Indigo, 84));
            brush.GradientStops.Add(new GradientStop(Colors.Violet, 1));

            Background = brush;

            InitializeComponent();
        }
    }
}

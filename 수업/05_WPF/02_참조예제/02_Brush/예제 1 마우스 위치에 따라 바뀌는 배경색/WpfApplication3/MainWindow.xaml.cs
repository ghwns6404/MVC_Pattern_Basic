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

namespace WpfApplication3
{
    /// <summary>
    /// MainWindow.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class MainWindow : Window
    {
        SolidColorBrush brush = new SolidColorBrush();
        public MainWindow()
        {
            InitializeComponent();
            Width = 400;
            Height = 400;
        }

        private void Window_MouseMove(object sender, MouseEventArgs e)
        {
            double width = ActualWidth - 2 * SystemParameters.ResizeFrameHorizontalBorderHeight;
            double height = ActualHeight - 2 * SystemParameters.ResizeFrameHorizontalBorderHeight
                - SystemParameters.CaptionHeight;

            //마우스의 위치가 변한 좌표를 가져와서 중심점을 기준으로 동일한 색을 나타내게 설정함(원형)
            Point ptMouse = e.GetPosition(this);
            Point ptCenter = new Point(width / 2, height / 2);
            Vector vectMouse = ptMouse - ptCenter;
            double angle = Math.Atan2(vectMouse.Y, vectMouse.X);
            Vector vectEllipse = new Vector(width / 2 * Math.Cos(angle), height / 2 * Math.Sin(angle));
            Byte byLevel = (byte)(255 * (1 - Math.Min(1, vectMouse.Length / vectEllipse.Length)));

            //색깔 채우고 배경에 표현
            Color clr = Color.FromRgb(byLevel, byLevel, byLevel);
            brush.Color = clr;
            Background = brush;
        }
    }
}

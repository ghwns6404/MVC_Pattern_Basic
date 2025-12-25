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

namespace AdjustMappingMode
{
    /// <summary>
    /// MainWindow.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class MainWindow : Window
    {
        LinearGradientBrush brush;
        public MainWindow()
        {
            Title = "Adjust the Gradient";
            brush = new LinearGradientBrush(Colors.White, Colors.Black, 0);
            brush.MappingMode = BrushMappingMode.Absolute;
            Background = brush;
            InitializeComponent();
        }

        private void Window_SizeChanged(object sender, SizeChangedEventArgs e)
        {
            double Width = ActualWidth - 2 * SystemParameters.ResizeFrameVerticalBorderWidth;
            double Height = ActualHeight - 2 * SystemParameters.ResizeFrameHorizontalBorderHeight - SystemParameters.CaptionHeight;

            Point ptCenter = new Point(Width / 2, Height / 2); //중간 좌표를 구한다.

            Vector vectDiag = new Vector(Width, -Height);       //좌측 하단부터 우측 상단까지의 대각선
            Vector vectPerp = new Vector(vectDiag.Y, -vectDiag.X); //vectDiag와 수직되는 대각선

            vectPerp.Normalize();               //정규화 시키기((0,0)~ (1,1))
            vectPerp *= Width * Height / vectDiag.Length;//vectPerp의 클라이언트 기준의 길이..
            //vectPerp의 클라이언트 기준의 길이를 구하는 이유는 길이를 알아야 Gradient가 그릴 좌표를 알 수 있기 때문이다.

            brush.StartPoint = ptCenter + vectPerp;     //해당하는 클라이언트의 시작과 끝 좌표를 대입한다.
            brush.EndPoint = ptCenter - vectPerp;
        }
    }
}

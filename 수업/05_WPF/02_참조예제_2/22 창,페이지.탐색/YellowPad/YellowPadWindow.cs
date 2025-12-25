//------------------------------------------------
// YellowPadWindow.cs (c) 2006 by Charles Petzold
//------------------------------------------------
using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Ink;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Shapes;

namespace Petzold.YellowPad
{
    public partial class YellowPadWindow : Window
    {
        // 노트 패트의 크기를 7x5 인치로 지정
        public static readonly double widthCanvas = 5 * 96;
        public static readonly double heightCanvas = 7 * 96;

        [STAThread]
        public static void Main()
        {
            Application app = new Application();
            app.Run(new YellowPadWindow());
        }
        public YellowPadWindow()
        {
            InitializeComponent();

            // 파란색 수평선을 1/4 인치 간격으로 그린다.
            double y = 96;

            while (y < heightCanvas)
            {
                Line line = new Line();
                line.X1 = 0;
                line.Y1 = y;
                line.X2 = widthCanvas;
                line.Y2 = y;
                line.Stroke = Brushes.LightBlue;
                inkcanv.Children.Add(line);

                y += 24;
            }

            // 태블릿 모드가 아니라면 Eraser-Mode menu를 비활성 시킨다.
            if (Tablet.TabletDevices.Count == 0)
                menuEraserMode.Visibility = Visibility.Collapsed;
        }
    }
}

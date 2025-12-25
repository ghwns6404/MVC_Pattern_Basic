using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Shapes;

namespace PaintTheButton
{
    public class PaintTheButton : Window
    {
        [STAThread]
        public static void Main()
        {
            Application app = new Application();
            app.Run(new PaintTheButton());
        }

        public PaintTheButton()
        {
            Title = "Paint the Button";

            // 버튼을 생성하고 창의 컨텐트로 지정
            // 버튼의 수평(Horizon) 수직(Vertical) 프로퍼티가 Center로 설정되었기 때문에 버튼의 크기는 Canvas 패널의 크기에 맞게 조절된다
            Button btn = new Button();
            btn.HorizontalAlignment = HorizontalAlignment.Center;
            btn.VerticalAlignment = VerticalAlignment.Center;
            Content = btn;

            // 1.5인치의 정사각형 Canvas를 생성하고 버튼의 컨텐트로 설정
            Canvas canv = new Canvas();
            canv.Width = 144;
            canv.Height = 144;
            btn.Content = canv;

            // Rectangle을 생성해 Cavas의 자식으로 지정
            Rectangle rect = new Rectangle();
            rect.Width = canv.Width;
            rect.Height = canv.Height;
            rect.RadiusX = 24;
            rect.RadiusY = 24;
            rect.Fill = Brushes.Blue;
            canv.Children.Add(rect);    // canv에 자식을 추가(rect) 클릭시 사각형이 눌리는 것처럼 보인다
            // Rectangle이 Canvas의 좌측 상단 모서리에 위치할 수 있게 함
            Canvas.SetLeft(rect, 0);
            Canvas.SetTop(rect, 0);

            // Poltgon을 생성해 Cavas의 자식으로 지정
            Polygon poly = new Polygon();
            poly.Fill = Brushes.Yellow;
            poly.Points = new PointCollection();  // PointCollection(다각형을 구성하는 점들이 저장)

            for (int i = 0; i < 5; i++)
            {
                double angle = i * 4 * Math.PI / 5;
                Point pt = new Point(48 * Math.Sin(angle) , -48*Math.Cos(angle));
                poly.Points.Add(pt);
            }

            canv.Children.Add(poly);
            //
            Canvas.SetLeft(poly , canv.Width/2);
            Canvas.SetTop(poly , canv.Height/2);
        }
    }
}
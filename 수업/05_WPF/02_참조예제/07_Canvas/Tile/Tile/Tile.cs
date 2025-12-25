using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Shapes;

namespace Hyun
{
    public class Tile : Canvas
    {
        const int SIZE = 64;    // 2/3인치
        const int BORD = 6;     // 1/16인치
        TextBlock txtblk;

        public Tile()
        {
            Width = SIZE;
            Height = SIZE;

            // 좌측과 상간의 밝게 반사된 경계
            Polygon poly = new Polygon();
            poly.Points = new PointCollection(new Point[]
            {
                new Point(0,0) , new Point(SIZE , 0) , 
                new Point(SIZE-BORD , BORD) , 
                new Point(BORD , BORD) , 
                new Point(BORD , SIZE-BORD) , new Point(0 , SIZE)
            });
            poly.Fill = SystemColors.ControlLightBrush;
            Children.Add(poly);

            // 우측과 하단의 그림자진 경계
            // PointCollection의 생성자에 전체 Point 배열을 정의
            poly = new Polygon();
            poly.Points = new PointCollection(new Point[]
            {
                new Point(SIZE , SIZE) , new Point(SIZE , 0) , 
                new Point(SIZE-BORD , BORD) , 
                new Point(SIZE-BORD , SIZE-BORD) , 
                new Point(BORD , SIZE-BORD) , new Point(0,SIZE)
            });
            poly.Fill = SystemColors.ControlDarkBrush;
            Children.Add(poly);

            // 중앙의 텍스트
            Border bord = new Border();
            bord.Width = SIZE - 2 * BORD;
            bord.Height = SIZE - 2 * BORD;
            bord.Background = SystemColors.ControlBrush;
            Children.Add(bord);
            SetLeft(bord, BORD);
            SetTop(bord, BORD);

            // 텍스트를 출력
            txtblk = new TextBlock();
            txtblk.FontSize = 32;
            txtblk.Foreground = SystemColors.ControlTextBrush;
            txtblk.HorizontalAlignment = HorizontalAlignment.Center;
            txtblk.VerticalAlignment = VerticalAlignment.Center;
            bord.Child = txtblk;
        }

        // 텍스트로 설정하는 public 프로퍼티
        public string Text
        {
            set { txtblk.Text = value;}
            get { return txtblk.Text; }

        }
    }
}
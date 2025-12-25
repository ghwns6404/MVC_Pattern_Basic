//-----------------------------------------------------
// DrawGraphicsOnBitmap.cs (c) 2006 by Charles Petzold
//-----------------------------------------------------
using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;

namespace Petzold.DrawGraphicsOnBitmap
{
    public class DrawGraphicsOnBitmap : Window
    {
        [STAThread]
        public static void Main()
        {
            Application app = new Application();
            app.Run(new DrawGraphicsOnBitmap());
        }
        public DrawGraphicsOnBitmap()
        {
            Title = "Draw Graphics on Bitmap";

            // 비트맵의 투명도를 예시하기 위해 배경을 설정함.
            Background = Brushes.Khaki;

            // RenderTargetBitmap 설정함.
            RenderTargetBitmap renderbitmap = new RenderTargetBitmap(100, 100, 96, 96, PixelFormats.Default);

            // DrawingVisual 객체를 생성함.
            DrawingVisual drawvis = new DrawingVisual();
            DrawingContext dc = drawvis.RenderOpen();      // Render 메소드 호출
            dc.DrawRoundedRectangle(Brushes.Blue, new Pen(Brushes.Red, 10),
                                    new Rect(25, 25, 50, 50), 10, 10);
            dc.Close();

            // RenderTargetBitmap 위에 DrawingVisual 객체를 그림.
            renderbitmap.Render(drawvis);

            // 이미지 객채 생성 -> 프로퍼티 설정
            Image img = new Image();
            img.Source = renderbitmap;

            // 출력
            Content = img;
        }
    }
}
//---------------------------------------------------
// TransformedButtons.cs (c) 2006 by Charles Petzold
//---------------------------------------------------
using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;

namespace Petzold.TransformedButtons
{
    public class TransformedButtons : Window
    {
        [STAThread]
        public static void Main()
        {
            Application app = new Application();
            app.Run(new TransformedButtons());
        }
        public TransformedButtons()
        {
            Title = "Transformed Buttons";

            // Create Canvas as content of window.
            Canvas canv = new Canvas();
            Content = canv;

            // Untransformed button.
            Button btn = new Button();
            btn.Content = "Untransformed";
            canv.Children.Add(btn);
            Canvas.SetLeft(btn, 50);
            Canvas.SetTop(btn, 100);

            // Translated button.
            btn = new Button();
            btn.Content = "Translated";
            btn.RenderTransform = new TranslateTransform(-100, 150);
            canv.Children.Add(btn);
            Canvas.SetLeft(btn, 200);
            Canvas.SetTop(btn, 100);

            // Scaled button.
            btn = new Button();
            btn.Content = "Scaled";

            //ScaleTransform를 이용하여 배율을 조정 
            btn.RenderTransform = new ScaleTransform(1.5, 4);
            canv.Children.Add(btn);
            Canvas.SetLeft(btn, 350);
            Canvas.SetTop(btn, 100);

            // Skewed button.
            btn = new Button();
            btn.Content = "Skewed";

            //SkewTransform를 이용하여 개체 2차원 기울이기
            btn.RenderTransform = new SkewTransform(0, 20);
            canv.Children.Add(btn);
            Canvas.SetLeft(btn, 500);
            Canvas.SetTop(btn, 100);

            // Rotated button.
            btn = new Button();
            btn.Content = "Rotated";

            //RotateTransform를 이용하여 지정한 점을 기준으로 개체를 시계 방향으로 회전
            btn.RenderTransform = new RotateTransform(-30);
            canv.Children.Add(btn);
            Canvas.SetLeft(btn, 650);
            Canvas.SetTop(btn, 100);
        }
    }
}
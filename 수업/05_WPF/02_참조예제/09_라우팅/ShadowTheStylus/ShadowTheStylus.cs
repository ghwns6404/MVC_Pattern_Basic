//------------------------------------------------
// ShadowTheStylus.cs (c) 2006 by Charles Petzold
//------------------------------------------------
using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Shapes;

namespace ShadowTheStylus
{
    public class ShadowTheStylus : Window
    {
        // 스타일러스 폴리라인에 대한 몇 가지 상수의 정의
        static readonly SolidColorBrush brushStylus = Brushes.Blue;
        static readonly SolidColorBrush brushShadow = Brushes.LightBlue;
        static readonly double widthStroke = 96 / 2.54;       // 1 cm
        static readonly Vector vectShadow = 
                        new Vector(widthStroke / 4, widthStroke / 4);   // 그림자효과

        // 스타일러스 이동에 대한 추가적인 필드
        Canvas canv;
        Polyline polyStylus, polyShadow;
        bool isDrawing;

        [STAThread]
        public static void Main()
        {
            Application app = new Application();
            app.Run(new ShadowTheStylus());
        }
        public ShadowTheStylus()
        {
            Title = "Shadow the Stylus";

            // Window 컨텐트로 사용할 Canvas 생성
            canv = new Canvas();
            Content = canv;
        }
        protected override void OnStylusDown(StylusDownEventArgs args)
        {
            base.OnStylusDown(args);
            Point ptStylus = args.GetPosition(canv);

            // 끝이 둥근 Polyline을 생성해 전경에 사용
            polyStylus = new Polyline();
            polyStylus.Stroke = brushStylus;
            polyStylus.StrokeThickness = widthStroke;
            polyStylus.StrokeStartLineCap = PenLineCap.Round;
            polyStylus.StrokeEndLineCap = PenLineCap.Round;
            polyStylus.StrokeLineJoin = PenLineJoin.Round;
            polyStylus.Points = new PointCollection();
            polyStylus.Points.Add(ptStylus);

            // 그림자용으로 쓸 Polyline
            polyShadow = new Polyline();
            polyShadow.Stroke = brushShadow;
            polyShadow.StrokeThickness = widthStroke;
            polyShadow.StrokeStartLineCap = PenLineCap.Round;
            polyShadow.StrokeEndLineCap = PenLineCap.Round;
            polyShadow.StrokeLineJoin = PenLineJoin.Round;
            polyShadow.Points = new PointCollection();
            polyShadow.Points.Add(ptStylus + vectShadow);

            // 전경의 모든 폴리라인 이전에 그림자 폴리라인을 삽입
            canv.Children.Insert(canv.Children.Count / 2, polyShadow);

            // 끝에 전경으로 사용할 폴리라인을 추가
            canv.Children.Add(polyStylus);

            CaptureStylus();
            isDrawing = true;
            args.Handled = true;
        }
        protected override void OnStylusMove(StylusEventArgs args)
        {
            base.OnStylusMove(args);

            if (isDrawing)
            {
                Point ptStylus = args.GetPosition(canv);
                polyStylus.Points.Add(ptStylus);
                polyShadow.Points.Add(ptStylus + vectShadow);
                args.Handled = true;
            }
        }
        protected override void OnStylusUp(StylusEventArgs args)
        {
            base.OnStylusUp(args);

            if (isDrawing)
            {
                isDrawing = false;
                ReleaseStylusCapture();
                args.Handled = true;
            }
        }
        protected override void OnTextInput(TextCompositionEventArgs args)
        {
            base.OnTextInput(args);

            // Esc 키를 누르면 그리기를 종료
            if (isDrawing && args.Text.IndexOf('\x1B') != -1)
            {
                ReleaseStylusCapture();
                args.Handled = true;
            }
        }
        protected override void OnLostStylusCapture(StylusEventArgs args)
        {
            base.OnLostStylusCapture(args);

            // 비정상적인 종료 : 해당 폴리라인을 제거
            if (isDrawing)
            {
                canv.Children.Remove(polyStylus);
                canv.Children.Remove(polyShadow);
                isDrawing = false;
            }
        }
    }
}

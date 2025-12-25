//--------------------------------------------
// DrawCircles.cs (c) 2006 by Charles Petzold
//--------------------------------------------
using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Shapes;

namespace Petzold.DrawCircles
{
    public class DrawCircles : Window
    {
        Canvas canv;

        // 그리는 것과 관련된 필드
        bool isDrawing;
        Ellipse elips;
        Point ptCenter;

        // 드래깅과 관련된 필드
        bool isDragging;
        FrameworkElement elDragging;
        Point ptMouseStart, ptElementStart;

        [STAThread]
        public static void Main()
        {
            Application app = new Application();
            app.Run(new DrawCircles());
        }
        public DrawCircles()
        {
            Title = "Draw Circles";
            Content = canv = new Canvas();
        }
        protected override void OnMouseLeftButtonDown(MouseButtonEventArgs args)
        {
            base.OnMouseLeftButtonDown(args);

            if (isDragging)
                return;

            // 새로운 타원 객체를 생성하고 캔버스에 추가
            ptCenter = args.GetPosition(canv);          //최초의 클릭으로 원의 중심 지정.
            elips = new Ellipse();
            elips.Stroke = SystemColors.WindowTextBrush;
            elips.StrokeThickness = 1;
            elips.Width = 0;
            elips.Height = 0;
            canv.Children.Add(elips);
            Canvas.SetLeft(elips, ptCenter.X);
            Canvas.SetTop(elips, ptCenter.Y);

            // 마우스를 캡처하고, 앞으로의 이벤트를 준비
            CaptureMouse();
            isDrawing = true;
        }
        protected override void OnMouseRightButtonDown(MouseButtonEventArgs args)
        {
            base.OnMouseRightButtonDown(args);

            if (isDrawing)
                return;

            // 클릭된 엘리먼트를 얻어오고, 앞으로의 이벤트를 준비
            ptMouseStart = args.GetPosition(canv);
            elDragging = canv.InputHitTest(ptMouseStart) as FrameworkElement;

            if (elDragging != null)
            {
                ptElementStart = new Point(Canvas.GetLeft(elDragging),
                                           Canvas.GetTop(elDragging));
                isDragging = true;
            }
        }
        protected override void OnMouseDown(MouseButtonEventArgs args)
        {
            base.OnMouseDown(args);

            if (args.ChangedButton == MouseButton.Middle)
            {
                Shape shape = canv.InputHitTest(args.GetPosition(canv)) as Shape;

                if (shape != null)
                    shape.Fill = (shape.Fill == Brushes.Red ? 
                                        Brushes.Transparent : Brushes.Red);
            }
        }
        protected override void OnMouseMove(MouseEventArgs args)
        {
            base.OnMouseMove(args);
            Point ptMouse = args.GetPosition(canv);

            // 타원의 크기 재조정
            if (isDrawing)
            {
                double dRadius = Math.Sqrt(Math.Pow(ptCenter.X - ptMouse.X, 2) +        // 마우스를 움직일 때는 마우스 커서 위치를 추적해 경계선을 지정한다.
                                           Math.Pow(ptCenter.Y - ptMouse.Y, 2));

                Canvas.SetLeft(elips, ptCenter.X - dRadius);
                Canvas.SetTop(elips, ptCenter.Y - dRadius);
                elips.Width = 2 * dRadius;
                elips.Height = 2 * dRadius;
            }
            // 타원을 이동
            else if (isDragging)
            {
                Canvas.SetLeft(elDragging, 
                    ptElementStart.X + ptMouse.X - ptMouseStart.X);
                Canvas.SetTop(elDragging, 
                    ptElementStart.Y + ptMouse.Y - ptMouseStart.Y);
            }
        }
        protected override void OnMouseUp(MouseButtonEventArgs args)
        {
            base.OnMouseUp(args);

            // 그리기 동작을 종료
            if (isDrawing && args.ChangedButton == MouseButton.Left)
            {
                elips.Stroke = Brushes.Blue;
                elips.StrokeThickness = Math.Min(24, elips.Width / 2);
                elips.Fill = Brushes.Red;

                isDrawing = false;
                ReleaseMouseCapture();
            }
            // 드래깅을 종료
            else if (isDragging && args.ChangedButton == MouseButton.Right)
            {
                isDragging = false;
            }
        }
        protected override void OnTextInput(TextCompositionEventArgs args)
        {
            base.OnTextInput(args);

            // Esc 키를 누르면 그리기나 드래깅을 종료
            if (args.Text.IndexOf('\x1B') != -1)
            {
                if (isDrawing)
                    ReleaseMouseCapture();

                else if (isDragging)
                {
                    Canvas.SetLeft(elDragging, ptElementStart.X);
                    Canvas.SetTop(elDragging, ptElementStart.Y);
                    isDragging = false;
                }
            }
        }
        protected override void OnLostMouseCapture(MouseEventArgs args)
        {
            base.OnLostMouseCapture(args);

            // 그리기의 비정상적 종료 : 자식 타원을 제거
            if (isDrawing)
            {
                canv.Children.Remove(elips);
                isDrawing = false;
            }
        }
    }
}
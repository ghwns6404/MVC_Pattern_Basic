//-----------------------------------------------
// MedievalButton.cs (c) 2006 by Charles Petzold
//-----------------------------------------------
using System;
using System.Globalization;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;

namespace Petzold.GetMedieval
{
    public class MedievalButton : Control
    {
        // 2개의 private 필드
        FormattedText formtxt;
        bool isMouseReallyOver;

        // 정적 읽기 전용 필드
        public static readonly DependencyProperty TextProperty;
        public static readonly RoutedEvent KnockEvent;
        public static readonly RoutedEvent PreviewKnockEvent;

        // 정적 생성자
        static MedievalButton()
        {
            // 의존 프로퍼티 등록
            TextProperty = 
                DependencyProperty.Register("Text", typeof(string), 
                                            typeof(MedievalButton), 
                    new FrameworkPropertyMetadata(" ", 
                            FrameworkPropertyMetadataOptions.AffectsMeasure));

            // 라우팅 이벤트 등록
            KnockEvent = 
                EventManager.RegisterRoutedEvent("Knock", RoutingStrategy.Bubble, 
                        typeof(RoutedEventHandler), typeof(MedievalButton));

            PreviewKnockEvent = 
                EventManager.RegisterRoutedEvent("PreviewKnock", 
                        RoutingStrategy.Tunnel,
                        typeof(RoutedEventHandler), typeof(MedievalButton));
        }

        // 의존 프로퍼티의 Public 인터페이스
        public string Text
        {
            set { SetValue(TextProperty, value == null ? " " : value); }
            get { return (string)GetValue(TextProperty); }
        }

        // 라우팅 이벤트의 Public 인터페이스
        public event RoutedEventHandler Knock
        {
            add { AddHandler(KnockEvent, value); }
            remove { RemoveHandler(KnockEvent, value); }
        }
        public event RoutedEventHandler PreviewKnock
        {
            add { AddHandler(PreviewKnockEvent, value); }
            remove { RemoveHandler(PreviewKnockEvent, value); }
        }

        // 버튼의 크기가 변경될 때 MeasureOverride가 호출됨
        protected override Size MeasureOverride(Size sizeAvailable)
        {
            formtxt = new FormattedText(
                    Text, CultureInfo.CurrentCulture, FlowDirection,
                    new Typeface(FontFamily, FontStyle, FontWeight, FontStretch),
                    FontSize, Foreground);

            // 크기를 계산할 때 Padding을 사용
            Size sizeDesired = new Size(Math.Max(48, formtxt.Width) + 4, 
                                                     formtxt.Height + 4);
            sizeDesired.Width += Padding.Left + Padding.Right;
            sizeDesired.Height += Padding.Top + Padding.Bottom;

            return sizeDesired;
        }

        // 버튼을 다시 그리기 위해 OnRender 호출
        protected override void OnRender(DrawingContext dc)
        {
            
            // 배경색 결정
            Brush brushBackground = SystemColors.ControlBrush;

            if (isMouseReallyOver && IsMouseCaptured)
                brushBackground = SystemColors.ControlDarkBrush;

            // 펜의 두께 결정
            Pen pen = new Pen(Foreground, IsMouseOver ? 2 : 1);

            // 둥근 모서리의 사각형을 그림
           dc.DrawRoundedRectangle(brushBackground, pen,
                                    new Rect(new Point(0, 0), RenderSize), 4, 4);

            // 전경색 결정
            formtxt.SetForegroundBrush(
                        IsEnabled ? Foreground : SystemColors.ControlDarkBrush);

            // 텍스트의 시작점 결정
            Point ptText = new Point(2, 2);
            
            switch (HorizontalContentAlignment)
            {
                case HorizontalAlignment.Left:
                    ptText.X += Padding.Left;
                    break;

                case HorizontalAlignment.Right:
                    ptText.X += RenderSize.Width - formtxt.Width - Padding.Right;
                    break;

                case HorizontalAlignment.Center:
                case HorizontalAlignment.Stretch:
                    ptText.X += (RenderSize.Width - formtxt.Width - 
                            Padding.Left - Padding.Right) / 2;
                    break;
            }
            switch (VerticalContentAlignment)
            {
                case VerticalAlignment.Top:
                    ptText.Y += Padding.Top;
                    break;

                case VerticalAlignment.Bottom:
                    ptText.Y += 
                        RenderSize.Height - formtxt.Height - Padding.Bottom;
                    break;

                case VerticalAlignment.Center:
                case VerticalAlignment.Stretch:
                    ptText.Y += (RenderSize.Height - formtxt.Height -
                            Padding.Top - Padding.Bottom) / 2;
                    break;
            }

            // 텍스트 출력
            dc.DrawText(formtxt, ptText);
        }
        // 버튼의 모습에 영향을 주는 Mouse 이벤트
        protected override void OnMouseEnter(MouseEventArgs args)
        {
            base.OnMouseEnter(args);
            InvalidateVisual();
        }
        protected override void OnMouseLeave(MouseEventArgs args)
        {
            base.OnMouseLeave(args);
            InvalidateVisual();
        }
        protected override void OnMouseMove(MouseEventArgs args)
        {
            base.OnMouseMove(args);

            Point pt = args.GetPosition(this);
            bool isReallyOverNow = (pt.X >= 0 && pt.X < ActualWidth &&
                                    pt.Y >= 0 && pt.Y < ActualHeight);
            if (isReallyOverNow != isMouseReallyOver)
            {
                isMouseReallyOver = isReallyOverNow;
                InvalidateVisual();
            }
        }

        // 'Knock' 이벤트를 발생시키는 작업의 시작
        protected override void OnMouseLeftButtonDown(MouseButtonEventArgs args)
        {
            base.OnMouseLeftButtonDown(args);
            CaptureMouse();
            InvalidateVisual();
            args.Handled = true;
        }

        // 이 이벤트가 실제로 'Knock' 이벤트 발생
        protected override void OnMouseLeftButtonUp(MouseButtonEventArgs args)
        {
            base.OnMouseLeftButtonUp(args);

            if (IsMouseCaptured)
            {
                if (isMouseReallyOver)
                {
                    OnPreviewKnock();
                    OnKnock();
                }
                args.Handled = true;
                Mouse.Capture(null);
            }
        }

        // 마우스 캡처가 종료되면 다시 그림
        protected override void OnLostMouseCapture(MouseEventArgs args)
        {
            base.OnLostMouseCapture(args);
            InvalidateVisual();
        }

        // Space bar 또는 Enter 키를 눌러도 버튼을 누르는 효과
        protected override void OnKeyDown(KeyEventArgs args)
        {
            base.OnKeyDown(args);
            if (args.Key == Key.Space || args.Key == Key.Enter)
                args.Handled = true;
        }
        protected override void OnKeyUp(KeyEventArgs args)
        {
            base.OnKeyUp(args);
            if (args.Key == Key.Space || args.Key == Key.Enter)
            {
                OnPreviewKnock();
                OnKnock();
                args.Handled = true;
            }
        }

        // OnKnock 메소드에서 'Knock' 이벤트 발생
        protected virtual void OnKnock()
        {
            RoutedEventArgs argsEvent = new RoutedEventArgs();
            argsEvent.RoutedEvent = MedievalButton.PreviewKnockEvent;
            argsEvent.Source = this;
            RaiseEvent(argsEvent);
        }

        // OnPreviewKnock 메소드에서 'PreviewKnock' 이벤트 발생
        protected virtual void OnPreviewKnock()
        {
            RoutedEventArgs argsEvent = new RoutedEventArgs();
            argsEvent.RoutedEvent = MedievalButton.KnockEvent;
            argsEvent.Source = this;
            RaiseEvent(argsEvent);
        }
    }
}

//----------------------------------------------
// RoundedButton.cs (c) 2006 by Charles Petzold
//----------------------------------------------
using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;

namespace Petzold.CalculateInHex
{
    public class RoundedButton : Control
    {
        // Private 필드
        RoundedButtonDecorator decorator;

        // Public 정적 ClickEvent.
        public static readonly RoutedEvent ClickEvent;

        // 정적 생성자
        static RoundedButton()
        {
            ClickEvent = EventManager.RegisterRoutedEvent("Click", RoutingStrategy.Bubble, typeof(RoutedEventHandler), typeof(RoundedButton));
        }

        // 생성자
        public RoundedButton()
        {
            decorator = new RoundedButtonDecorator();   
            AddVisualChild(decorator);
            AddLogicalChild(decorator);
        }

        // Public 프로퍼티
        public UIElement Child
        {
            set { decorator.Child = value; }
            get { return decorator.Child; }
        }
        public bool IsPressed
        {
            set { decorator.IsPressed = value; }
            get { return decorator.IsPressed; }
        }

        // Public 이벤트
        public event RoutedEventHandler Click
        {
            add { AddHandler(ClickEvent, value); }
            remove { RemoveHandler(ClickEvent, value); }
        }

        // 오버라이딩하는 프로퍼티와 메소드
        protected override int VisualChildrenCount
        {
            get { return 1; }
        }

        protected override Visual GetVisualChild(int index)
        {
            if (index > 0)
                throw new ArgumentOutOfRangeException("index");

            return decorator;
        }

        protected override Size MeasureOverride(Size sizeAvailable)
        {
            decorator.Measure(sizeAvailable);
            return decorator.DesiredSize;
        }
        protected override Size ArrangeOverride(Size sizeArrange)
        {
            decorator.Arrange(new Rect(new Point(0, 0), sizeArrange));
            return sizeArrange;
        }

        protected override void OnMouseMove(MouseEventArgs args)
        {
            base.OnMouseMove(args);
            
            if (IsMouseCaptured)
                IsPressed = IsMouseReallyOver;
        }

        protected override void OnMouseLeftButtonDown(MouseButtonEventArgs args)
        {
            base.OnMouseLeftButtonDown(args);
            CaptureMouse();
            IsPressed = true;
            args.Handled = true;
        }
        protected override void OnMouseLeftButtonUp(MouseButtonEventArgs args)
        {
            base.OnMouseRightButtonUp(args);

            if (IsMouseCaptured)
            {
                if (IsMouseReallyOver)
                    OnClick();

                Mouse.Capture(null);
                IsPressed = false;
                args.Handled = true;
            }
        }

        bool IsMouseReallyOver
        {
            get
            {
                Point pt = Mouse.GetPosition(this);

                // 마우스의 좌표값이 렌더링 좌표안에 있는지 확인후 Return
                return (pt.X >= 0 && pt.X < ActualWidth &&
                        pt.Y >= 0 && pt.Y < ActualHeight);
            }
        }

        // Click 이벤트를 발생시키는 메소드
        protected virtual void OnClick()
        {
            RoutedEventArgs argsEvent = new RoutedEventArgs();
            argsEvent.RoutedEvent = RoundedButton.ClickEvent;
            argsEvent.Source = this;
            RaiseEvent(argsEvent);
        }
    }
}


//-------------------------------------------------------
// RoundedButtonDecorator.cs (c) 2006 by Charles Petzold
//-------------------------------------------------------
using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;

namespace Petzold.CalculateInHex
{
    public class RoundedButtonDecorator : Decorator
    {
        // Public 의존 프로퍼티
        public static readonly DependencyProperty IsPressedProperty;

        // 정적 생성자
        static RoundedButtonDecorator()
        {
            // 의존 프로퍼티를 레지스터에 등록
            IsPressedProperty = 
                DependencyProperty.Register("IsPressed", typeof(bool),
                        typeof(RoundedButtonDecorator),
                        new FrameworkPropertyMetadata(false, 
                        FrameworkPropertyMetadataOptions.AffectsRender));
        }

        // Public 프로퍼티
        public bool IsPressed
        {
            set { SetValue(IsPressedProperty, value); }
            get { return (bool)GetValue(IsPressedProperty); }
        }
        
        // MeasureOverride 오버라이딩
        protected override Size MeasureOverride(Size sizeAvailable)
        {
            Size szDesired = new Size(2, 2);
            sizeAvailable.Width -= 2;
            sizeAvailable.Height -= 2;

            if (Child != null)          // Child - Decorator 로 부터 상속
            {
                Child.Measure(sizeAvailable);
                szDesired.Width += Child.DesiredSize.Width;
                szDesired.Height += Child.DesiredSize.Height;
            }
            return szDesired;
        }
        // ArrangeOverride 오버라이딩
        protected override Size ArrangeOverride(Size sizeArrange)
        {
            if (Child != null)
            {
                Point ptChild = 
                    new Point(Math.Max(1, (sizeArrange.Width - 
                                            Child.DesiredSize.Width) / 2), 
                              Math.Max(1, (sizeArrange.Height - 
                                            Child.DesiredSize.Height) / 2));

                Child.Arrange(new Rect(ptChild, Child.DesiredSize));
            }
            return sizeArrange;
        }
        // OnRender 오버라이딩
        protected override void OnRender(DrawingContext dc)
        {
            RadialGradientBrush brush = new RadialGradientBrush(
                    IsPressed ? SystemColors.ControlDarkColor :
                                SystemColors.ControlLightLightColor,
                    SystemColors.ControlColor);

            brush.GradientOrigin = IsPressed ? new Point(0.75, 0.75) :
                                               new Point(0.25, 0.25);
            dc.DrawRoundedRectangle(brush,
                    new Pen(SystemColors.ControlDarkDarkBrush, 1),
                    new Rect(new Point(0, 0), RenderSize),
                             RenderSize.Height / 2, RenderSize.Height / 2);
        }
    }
}

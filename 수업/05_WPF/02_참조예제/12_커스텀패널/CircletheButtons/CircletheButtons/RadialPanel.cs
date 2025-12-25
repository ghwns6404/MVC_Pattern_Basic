using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Ink;
using System.Windows.Controls;
using System.Windows.Media;

// 방사형의 패널 . . 자식을 원형으로 배열하는 패널이다..
namespace CircletheButtons
{
    //열거형 
    public enum RadialPanelOrientation
    {
        ByWidth,ByHeight
    }
    
    public class RadialPanel:Panel
    {
        //의존프로퍼티
        public static readonly DependencyProperty OrientationProperty;


         bool showPieLines;
        double angleEach;         //각 자식의 각도
        Size sizeLargest;         //가장 큰 자식 크기
        double radious;           //원의 반지름
        double outerEdgeFromCenter;
        double innerEdgeFronCenter;

        //정적 생성자에서 Orientation 의존 프로퍼티 생성
        static RadialPanel()
        {
            OrientationProperty = DependencyProperty.Register("Orientation", typeof(RadialPanelOrientation),
                typeof(RadialPanel),new FrameworkPropertyMetadata(RadialPanelOrientation.ByWidth,FrameworkPropertyMetadataOptions.AffectsMeasure));
        }

        //Orientation 프로퍼티
        public RadialPanelOrientation Orientation
        {
            set { SetValue(OrientationProperty, value); }
            get { return (RadialPanelOrientation)GetValue(OrientationProperty);  }
        }

        //ShowPieLines 프로퍼티
        public bool ShowPieLines
        {
            set
            {
                showPieLines = value;
            }

            get
            {
                return showPieLines;
            }
        }

        //MeasureOverride 오버라이딩
        protected override Size MeasureOverride(Size availableSize)
        {
            if (InternalChildren.Count == 0)
                return new Size(0,0);

            //360에서 자식의 수를 나누어서 angleEach를 계산..
            angleEach = 360.0 / InternalChildren.Count;
            sizeLargest = new Size(0,0);

            //자식 크기 검사, 가장 큰 가로와 세로 길이를 찾음..
            foreach (UIElement child in InternalChildren)
            {
                //각 자식에 Measure를 호출
                //루프에서 모든 자식에 대해 무한대 크기의 인자로 Meeasure 호출...
                child.Measure(new Size(Double.PositiveInfinity,Double.PositiveInfinity));

                sizeLargest.Width = Math.Max(sizeLargest.Width,child.DesiredSize.Width);

                sizeLargest.Height = Math.Max(sizeLargest.Height,child.DesiredSize.Height);
            }
            if (Orientation == RadialPanelOrientation.ByWidth)
            {
                //중심에서 엘리먼트 변까지의 거리를 계산
                innerEdgeFronCenter = sizeLargest.Width / 2 / Math.Tan(Math.PI*angleEach/360);
                outerEdgeFromCenter = innerEdgeFronCenter + sizeLargest.Height;

                radious = Math.Sqrt(Math.Pow(sizeLargest.Width/2,2)+Math.Pow(outerEdgeFromCenter,2));
            }
            else
            {
                //중심에서 엘리먼트 변까지의 거리를 계산
                innerEdgeFronCenter=sizeLargest.Height/2/Math.Tan(Math.PI*angleEach/360);
                outerEdgeFromCenter=innerEdgeFronCenter +sizeLargest.Width;

                radious = Math.Sqrt(Math.Pow(sizeLargest.Height/2,2)+Math.Pow(outerEdgeFromCenter,2));
            }
            return new Size(2*radious,2*radious);
        }

        //ArrangeOverride 오버라이딩
        protected override Size ArrangeOverride(Size finalSize)
        {
            double angleChild = 0;
            Point ptCenter = new Point(finalSize.Width/2,finalSize.Height/2);
            double multiplier = Math.Min(finalSize.Width/(2*radious),finalSize.Height/(2*radious));

            foreach (UIElement child in InternalChildren)
            {
                child.RenderTransform = Transform.Identity;

                if (Orientation == RadialPanelOrientation.ByWidth)
                {
                    child.Arrange(new Rect(ptCenter.X-multiplier*sizeLargest.Width/2,
                                           ptCenter.Y-multiplier*outerEdgeFromCenter,
                                           multiplier*sizeLargest.Width,
                                           multiplier*sizeLargest.Height));
                }

                else
                {
                    child.Arrange(new Rect(ptCenter.X + multiplier * innerEdgeFronCenter, 
                        ptCenter.Y - multiplier * sizeLargest.Height / 2,
                        multiplier * sizeLargest.Width, multiplier * sizeLargest.Height));
                }
                Point pt = TranslatePoint(ptCenter, child);
                child.RenderTransform = new RotateTransform(angleChild, pt.X, pt.Y);

                angleChild += angleEach;
            }
            return finalSize;
        }

        //그려주기..
        protected override void OnRender(DrawingContext dc)
        {
            base.OnRender(dc);

            if (showPieLines)
            {
                Point ptCenter = new Point(RenderSize.Width/2,RenderSize.Height/2);
                double multiplier = Math.Min(RenderSize.Width/(2*radious),RenderSize.Height/(2*radious));
                Pen pen = new Pen(SystemColors.WindowTextBrush,1);
                pen.DashStyle = DashStyles.Dash;

                dc.DrawEllipse(null,pen,ptCenter,multiplier*radious,multiplier*radious);

                double angleChild= -angleEach/2;

                if (Orientation == RadialPanelOrientation.ByWidth)
                    angleChild += 90;

                foreach (UIElement child in InternalChildren)
                {
                    dc.DrawLine(pen,ptCenter,
                        new Point(ptCenter.X+multiplier*radious*Math.Cos(2*Math.PI*angleChild/360),
                        ptCenter.Y+multiplier*radious*
                        Math.Sin(2*Math.PI*angleChild/360)));
                    angleChild += angleEach;
                }
            }
        }
    }
}

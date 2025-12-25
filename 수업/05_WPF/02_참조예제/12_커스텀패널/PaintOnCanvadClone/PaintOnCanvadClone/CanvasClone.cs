using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Controls;

namespace PaintOnCanvadClone
{
    //===============================================================================================
    //=DockPanel,Grid,Canvas에는 모두 첨부 프로퍼티가 있다.Canvas에 얼리먼트를 추가할 때 그 엘리먼트를 
    //=적용하기 위해서 다음과 같이 2개의 첨부 프로퍼티도 설정했다
    //=============================
    //=  canv.Children.Add(el);   =
    //=  Canvas.SetLeft(el,100);  =
    //=  Canvas.SetTop(el,150);   =
    //=============================
    //=다음에 있는 CanvasClone클래스는 Canvas와 매우 닯았다
    //================================================================================================
    class CanvasClone:Panel
    {
        public static readonly DependencyProperty LeftProperty;
        public static readonly DependencyProperty TopProperty;
      

        static CanvasClone()
        {

            //Left,Top첨부 프로퍼티만 구현... Right와 Bottom은 없다...
            //의존 프로퍼티를 첨부 프로퍼티로 등록
            //기본값은 0 이며,변경이 되면 부모의 배치에 영향을 줌 
            LeftProperty = DependencyProperty.RegisterAttached("Left",
                typeof(double), typeof(CanvasClone),
                new FrameworkPropertyMetadata(0.0, FrameworkPropertyMetadataOptions.AffectsParentArrange));

            TopProperty = DependencyProperty.RegisterAttached("Top",
                typeof(double), typeof(CanvasClone),
                new FrameworkPropertyMetadata(0.0, FrameworkPropertyMetadataOptions.AffectsParentArrange));
  
        }

        

        //첨부 프로퍼티를 얻어오고 , 설정하는 정적 메소드
        public static void SetLeft(DependencyObject obj, double value)
        {
            obj.SetValue(LeftProperty,value);
        }
        public static double GetLeft(DependencyObject obj)
        {
            return (double)obj.GetValue(LeftProperty);
        }
        public static void SetTop(DependencyObject obj, double value)
        {
            obj.SetValue(TopProperty,value);
        }
        public static double GetTop(DependencyObject obj)
        {
            return (double)obj.GetValue(TopProperty);
        }

        //MeasureOverride에서 하는 일이 무한대 크기로 각 자식의 Measure를 호출하는 것이다.
        //이는 Canvas와 일치한다
        //Measure를 호출하는 이유는 자식이 DesiredSize프로퍼티를 계산할 수 있게 하기 위해서다
        protected override Size MeasureOverride(Size availableSize)
        {
            foreach (UIElement child in InternalChildren)
                child.Measure(new Size(Double.PositiveInfinity, Double.PositiveInfinity));
                       //레이아웃크기 측정..(기본값 0 , 0 ) 반환
            return base.MeasureOverride(availableSize);
        }

        protected override Size ArrangeOverride(Size finalSize)
        {
            foreach (UIElement child in InternalChildren)
                child.Arrange(new Rect(new Point(GetLeft(child), GetTop(child)), child.DesiredSize));
                        //자식요소 배치
            return base.ArrangeOverride(finalSize);
        }
    }
}

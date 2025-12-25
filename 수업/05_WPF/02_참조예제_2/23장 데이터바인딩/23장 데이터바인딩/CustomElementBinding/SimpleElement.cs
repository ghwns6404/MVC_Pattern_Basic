//----------------------------------------------
// SimpleElement.cs (c) 2006 by Charles Petzold
//----------------------------------------------
using System;
using System.Globalization;
using System.Windows;
using System.Windows.Media;

namespace Petzold.CustomElementBinding
{
    class SimpleElement : FrameworkElement
    {
        // DependencyProperty 정의
        public static DependencyProperty NumberProperty;

        // 정적 생성자에 DependencyProperty 생성
        static SimpleElement()
        {
            NumberProperty =
                DependencyProperty.Register("Number",                 // DependencyProperty의 이름
                                        typeof(double),             // 속성이 포함할 수 있는 값의 타입
                                        typeof(SimpleElement),       // 종속성 속성 소유자 타입
                    new FrameworkPropertyMetadata(0.0,              // 추가 속성 옵션 메타데이터
                    FrameworkPropertyMetadataOptions.AffectsRender));
                                
        }

        // DependencyProperty를 CLR 프로퍼티로 노출
        public double Number
        {
            set { SetValue(NumberProperty, value); }
            get { return (double)GetValue(NumberProperty); }
        }

        // MeasureOverride를 오버라이딩해 크기를 하드 코딩
        protected override Size MeasureOverride(Size sizeavailable)
        {
            return new Size(200, 50);
        }

        // Number 프로퍼티를 보여주는 OnRender
        protected override void OnRender(DrawingContext dc)    
        {
            dc.DrawText(
                new FormattedText(Number.ToString(),
                        CultureInfo.CurrentCulture, FlowDirection.LeftToRight,
                        new Typeface("Times New Roman"), 12,
                        SystemColors.WindowTextBrush),
                new Point(0, 0)); 
        }
    }
}

//------------------------------------------
// ColorCell.cs (c) 2006 by Charles Petzold
//------------------------------------------
using System;
using System.Windows;
using System.Windows.Media;

namespace Petzold.SelectColor
{
    class ColorCell : FrameworkElement
    {
        // Private 필드
        static readonly Size sizeCell = new Size(20, 20);
        DrawingVisual visColor;
        Brush brush;

        // 의존 프로퍼티
        public static readonly DependencyProperty IsSelectedProperty;
        public static readonly DependencyProperty IsHighlightedProperty;
 
        static ColorCell()
        {
            // 의존 프로퍼티 등록
            IsSelectedProperty = 
                DependencyProperty.Register("IsSelected", typeof(bool), 
                        typeof(ColorCell), new FrameworkPropertyMetadata(false, 
                                FrameworkPropertyMetadataOptions.AffectsRender));

            IsHighlightedProperty = 
                DependencyProperty.Register("IsHighlighted", typeof(bool), 
                        typeof(ColorCell), new FrameworkPropertyMetadata(false, 
                                FrameworkPropertyMetadataOptions.AffectsRender));
        }

        // 프로퍼티
        public bool IsSelected
        {
            set { SetValue(IsSelectedProperty, value); }
            get { return (bool)GetValue(IsSelectedProperty); }
        }
        public bool IsHighlighted
        {
            set { SetValue(IsHighlightedProperty, value); }
            get { return (bool)GetValue(IsHighlightedProperty); }
        }
        public Brush Brush
        {
            get { return brush; }
        }

        // Color를 인자로 하는 생성자
        public ColorCell(Color clr)
        {
            // DrawingVisual을 생성해 필드에 저장
            visColor = new DrawingVisual();
            DrawingContext dc = visColor.RenderOpen();

            // Color 인자를 가지고 사각형 그리기
            Rect rect = new Rect(new Point(0, 0), sizeCell);
            rect.Inflate(-4, -4); 
            Pen pen = new Pen(SystemColors.ControlTextBrush, 1);
            brush = new SolidColorBrush(clr);
            dc.DrawRectangle(brush, pen, rect);
            dc.Close();

            // AddVisualChild는 이벤트 라우팅에 필요함!
            AddVisualChild(visColor);
            AddLogicalChild(visColor);
        }

        // VisualChild에 관련된 protected 프로퍼티와 메소드를 오버라이딩
        // Visual의 존재를 지시하기 위해 1을 반환
        protected override int VisualChildrenCount
        {
            get
            {
                return 1;
            }
        }

        // Visual 그 자체를 반환한다.
        protected override Visual GetVisualChild(int index)
        {
            if (index > 0)
                throw new ArgumentOutOfRangeException("index");

            return visColor;
        }


        // 엘리먼트의 크기와 렌더링과 관련된 protected 메소드를 오버라이딩
        protected override Size MeasureOverride(Size sizeAvailable)
        {
            return sizeCell;
        }

        protected override void OnRender(DrawingContext dc)
        {
            Rect rect = new Rect(new Point(0, 0), RenderSize);
            
            // 사각형을 지정한 크기만큼 축소하거나 확장
            rect.Inflate(-2, -2);   
            Pen pen = new Pen(SystemColors.HighlightBrush, 1);

            if (IsHighlighted)      // 항목의 강조 표시 여부
                dc.DrawRectangle(SystemColors.ControlDarkBrush, pen, rect);
            else if (IsSelected)    // 선택 여부
                dc.DrawRectangle(SystemColors.ControlLightBrush, pen, rect);
            else
                dc.DrawRectangle(Brushes.Transparent, null, rect);
        }
    }
}

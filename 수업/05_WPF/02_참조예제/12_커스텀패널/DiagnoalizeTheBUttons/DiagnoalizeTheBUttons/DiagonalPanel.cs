using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Input;
using System.Windows.Controls;
using System.Windows.Media;

namespace DiagnoalizeTheBUttons
{
    //이전 예제까지만 해도 Panel을 상속받았는데...이제부터는
    //FrameworkElement를 상속받는 클래스로, 자식의 컬력션 직접관리하고,
    //좌측 상단에서부터 우측 하단까지의 대각선으로 자식을 배치한다.
    class DiagonalPanel:FrameworkElement
    {
        //private children 컬렉션
        List<UIElement> children=new List<UIElement>();

        //Private 필드
        Size sizeChildrenTotal;

        //의존 프로퍼티
        public static readonly DependencyProperty BackgroundProperty;

        //정적 생성자에서 Background 의존 프로퍼티 등록
        static DiagonalPanel()
        {
            BackgroundProperty = DependencyProperty.Register("Background",typeof(Brush),typeof(DiagonalPanel),
                new FrameworkPropertyMetadata(null,FrameworkPropertyMetadataOptions.AffectsRender) );
        }

        //Background 프로퍼티 - (의존 프로퍼티로 지원됨)
        public Brush Background
        {
            set { SetValue(BackgroundProperty, value); }
            get { return (Brush)GetValue(BackgroundProperty); }
        }

        //자식 컬렉션에 접근하는 메소드=======================================
        public void Add(UIElement el)
        {   
            children.Add(el);  //끝부분에 추가

            //생각 안나시는분을 위하여...p.307참고..====
            AddVisualChild(el);// 부모 / 자식 관계정의
            AddLogicalChild(el);// 논리적 트리에 추가
            //=======================================
           InvalidateMeasure();// 레이아웃 무효화~
        }
        public void Remove(UIElement el)
        {
            children.Remove(el);    //맨처음 발견되는 특정 개체 제거
            //=================================================
            RemoveVisualChild(el);  //부모 / 자식 관계 제거...ㄷㄷ
            RemoveLogicalChild(el); // 트리에 제공된 개체 제거
            //=================================================
            InvalidateMeasure();    
        }
        public int IndexOf(UIElement el)
        {
            return children.IndexOf(el);    //지정된 개체 검색 및 처음으로 검색한 개체 반환
            //한마디로!!! 지정된 요소의 위치 반환
        }
        //===================================================================

        //오버라이딩 하는 프로퍼티와 메서드들~
        protected override int VisualChildrenCount
        {
            get
            {
                return children.Count;
            }
        }

        protected override Visual GetVisualChild(int index)
        {
            if (index >= children.Count)
                throw new ArgumentOutOfRangeException("Index");
            return children[index];
        }
        
        //호출될때 자식의 Measure 메서드에 어떤size값을 넘겨야할지 명확하지가 않아서.
        //Measure메서드에 sizeAvailable 인자 전체를 넘김..
        //한 자식이 모든공간 독점 사용 가능이 있기에 Measure에 무한대 크기의 size 인자를 준다.
        //자식이 크기를 직접 결정한다...
        protected override Size MeasureOverride(Size availableSize)
        {
            sizeChildrenTotal = new Size(0,0);
            foreach (UIElement child in children)
            {
                child.Measure(new Size(Double.PositiveInfinity,Double.PositiveInfinity));
                //자식들 대각선 배치!!!
                sizeChildrenTotal.Width += child.DesiredSize.Width;
                sizeChildrenTotal.Height += child.DesiredSize.Height;
            }
            return base.MeasureOverride(availableSize);
        }

        protected override Size ArrangeOverride(Size finalSize)
        {
            Point ptChild = new Point(0,0);
            foreach (UIElement child in children)
            {
                Size sizeChild = new Size(0,0);

                sizeChild.Width = child.DesiredSize.Width * (finalSize.Width / sizeChildrenTotal.Width);
                sizeChild.Height = child.DesiredSize.Height * (finalSize.Height / sizeChildrenTotal.Height);

                child.Arrange(new Rect(ptChild,sizeChild));

                ptChild.X += sizeChild.Width;
                ptChild.Y += sizeChild.Height;
            }
            return finalSize;
        }

        //배경 브러시로 전체 표면을 칠한다.
        protected override void OnRender(DrawingContext dc)
        {
            dc.DrawRectangle(Background, null, new Rect(new Point(0, 0), RenderSize));
        }
    }
}

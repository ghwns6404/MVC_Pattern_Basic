//-------------------------------------------------
// EllipseWithChild.cs (c) 2006 by Charles Petzold
//-------------------------------------------------
using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;

namespace Petzold.EncloseElementInEllipse
{
    public class EllipseWithChild : Petzold.RenderTheBetterEllipse.BetterEllipse
    {
        UIElement child;

        // Public Child 프로퍼티
        public UIElement Child
        {
            set
            {
                if (child != null)
                {
                    RemoveVisualChild(child);
                    RemoveLogicalChild(child);
                }
                if ((child = value) != null)
                {
                    AddVisualChild(child);          // 부모 - 자식 관계를 설정.
                    AddLogicalChild(child);         // 제공된 개체를 이 요소의 논리적 트리에 추가.
                }
            }
            get
            {
                return child;
            }
        }

        // VisualChildrenCount 오버라이딩. Child가 null이 아니면 1을 반환
        protected override int VisualChildrenCount
        {
            get
            {
                return Child != null ? 1 : 0;
            }
        }

        // GetVisualChildren 오버라이딩. Child를 반환
        protected override Visual GetVisualChild(int index)			// 인덱스에 대응하는 자식을 반환할 수 있음.
        {
            if (index > 0 || Child == null)					        // 이 메소드는 null을 반환해서는 안된다고 나와있음.
                throw new ArgumentOutOfRangeException("index");		// 그래서 인덱스가 부정확할 경우에는 예외 상황을 던져야 함.

            return Child;
        }

        // MeasureOverride 오버라이딩. Child의 Measure 메소드를 호출
        // 자식 Element에 필요한 크기를 측정하고 크기를 결정한다.
        protected override Size MeasureOverride(Size sizeAvailable)
        {
            Size sizeDesired = new Size(0, 0);					    // EllipseWithChild에서 이 메소드는 크기가 0인 sizeDesired 객체를 가지고 시작.

            if (Stroke != null)
            {
                sizeDesired.Width += 2 * Stroke.Thickness;			// 그 후 타원의 경계에 2를 곱한 값을 sizeDesired에 더한다.
                sizeDesired.Height += 2 * Stroke.Thickness;			// 적어도 경계선 전체를 출력해야 함을 의미.
                
                sizeAvailable.Width = Math.Max(0, sizeAvailable.Width - 2 * Stroke.Thickness);		// 0보다 작아지지는 않게 함.
                sizeAvailable.Height = Math.Max(0, sizeAvailable.Height - 2 * Stroke.Thickness);
            }

            if (Child != null)						                // Child 프로퍼티가 null이 아니면 MeasureOverride에서는 계속해서 조정된
            {
                Child.Measure(sizeAvailable);				        // sizeAvailable을 인자로 해서 자식의 Measure 메소드를 호출(그 자식의 MeasureOverride 호출).
                                                                    // 부모 요소가 자식요소에게 sizeAvailable만큼 크기를 할당한다.
                                                                    // DesiredSize의 프로퍼티를 계산하기 위해서 MeasureOverride 반환값을 이용.

                                                                    // DesiredSize - 엘리먼트에 필요한 레이아웃 크기 Margin(여백) 포함
                sizeDesired.Width += Child.DesiredSize.Width;		// Measure 메소드가 DesiredSize 프로퍼티를 업데이트하는 책임을 지님.
                sizeDesired.Height += Child.DesiredSize.Height;     // DesiredSize 프로퍼티를 참조하여 자식에게 필요한 공간이 어느정도인지 결정.
            }

            return sizeDesired;
        }

        // ArrangeOverride 오버라이딩. Child의 Arrange 메소드를 호출
        // 자식들을 배열 한다.
        protected override Size ArrangeOverride(Size sizeFinal)
        {
            if (Child != null)
            {
                Rect rect = new Rect(
                    new Point((sizeFinal.Width - Child.DesiredSize.Width) / 2,
                              (sizeFinal.Height - Child.DesiredSize.Height) / 2),
                              Child.DesiredSize);
                Child.Arrange(rect);
            }
            return sizeFinal;           // 레이아웃에서 요소가 정렬된 후 사용되는 실제 크기
        }
    }
}

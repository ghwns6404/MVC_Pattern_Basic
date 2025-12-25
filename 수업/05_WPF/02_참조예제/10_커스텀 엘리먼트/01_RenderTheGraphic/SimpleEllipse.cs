//----------------------------------------------
// SimpleEllipse.cs (c) 2006 by Charles Petzold
//----------------------------------------------
using System;
using System.Windows;
using System.Windows.Media;

namespace Petzold.RenderTheGraphic
{
    class SimpleEllipse : FrameworkElement
    {
        protected override void OnRender(DrawingContext dc)
        {
            //DrawingContext : 가장 낮은 수준의 그리기와 관련 메소드
	        //또 다른 의미는 dc or drawingContext
            dc.DrawEllipse(Brushes.Blue, //타원의 내부
                new Pen(Brushes.Red, 24),//타원의 경계선
                new Point(RenderSize.Width / 2, RenderSize.Height / 2),//타원의 중심을 가리키는 Point객체
                RenderSize.Width / 2, RenderSize.Height / 2);//수평 반경과 수직 반경
        }
    }
}
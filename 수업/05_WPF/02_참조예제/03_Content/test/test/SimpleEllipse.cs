using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Media;

namespace test
{
    class SimpleEllipse : FrameworkElement
    {
        protected override void OnRender(System.Windows.Media.DrawingContext drawingContext)
        {
            //base.OnRender(drawingContext);
            drawingContext.DrawEllipse(Brushes.Blue, new Pen(Brushes.Red, 24),
                new Point(RenderSize.Width / 2, RenderSize.Height / 2),
                RenderSize.Width / 2, RenderSize.Height / 2);
        }
    }
}

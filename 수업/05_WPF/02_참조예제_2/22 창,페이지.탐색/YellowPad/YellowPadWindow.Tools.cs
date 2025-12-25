//------------------------------------------------------
// YellowPadWindow.Tools.cs (c) 2006 by Charles Petzold
//------------------------------------------------------
using System;
using System.Windows;
using System.Windows.Controls;

namespace Petzold.YellowPad
{
    public partial class YellowPadWindow : Window
    {
        // StylusToolDialog를 띄우고 DrawingAttributes property를 사용한다.
        void StylusToolOnClick(object sender, RoutedEventArgs args)
        {
            StylusToolDialog dlg = new StylusToolDialog();
            dlg.Owner = this;
            dlg.DrawingAttributes = inkcanv.DefaultDrawingAttributes;

            if ((bool)dlg.ShowDialog().GetValueOrDefault())
            {
                inkcanv.DefaultDrawingAttributes = dlg.DrawingAttributes;
            }
        }
        // EraserToolDialog 를 띄우고 EraserShape property를 사용한다.
        void EraserToolOnClick(object sender, RoutedEventArgs args)
        {
            EraserToolDialog dlg = new EraserToolDialog();
            dlg.Owner = this;
            dlg.EraserShape = inkcanv.EraserShape;

            if ((bool)dlg.ShowDialog().GetValueOrDefault())
            {
                inkcanv.EraserShape = dlg.EraserShape;
            }
        }
    }
}

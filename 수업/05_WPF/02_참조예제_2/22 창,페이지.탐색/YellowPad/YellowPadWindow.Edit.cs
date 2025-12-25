//-----------------------------------------------------
// YellowPadWindow.Edit.cs (c) 2006 by Charles Petzold
//-----------------------------------------------------
using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Ink;
using System.Windows.Input;

namespace Petzold.YellowPad
{
    public partial class YellowPadWindow : Window
    {
        // 선이 선택되어 있으면 Format 항목 활성화.
        void EditOnOpened(object sender, RoutedEventArgs args)
        {
            itemFormat.IsEnabled = inkcanv.GetSelectedStrokes().Count > 0;
        }
        // 선이 선택되어 있으면 Cut, Copy, Delete 항목 활성화.
        void CutCanExecute(object sender, CanExecuteRoutedEventArgs args)
        {
            args.CanExecute = inkcanv.GetSelectedStrokes().Count > 0;
        }
        // InkCanvas내 메소드를 이용해 Cut과 Copy 구현.
        void CutOnExecuted(object sender, ExecutedRoutedEventArgs args)
        {
            inkcanv.CutSelection();
        }
        void CopyOnExecuted(object sender, ExecutedRoutedEventArgs args)
        {
            inkcanv.CopySelection();
        }
        // 클리보드 내 항목이 붙여 넣기가 가능하면 Paste 항목 활성화.

        void PasteCanExecute(object sender, CanExecuteRoutedEventArgs args)
        {
            args.CanExecute = inkcanv.CanPaste();
        }
        // InkCanvas내의 메소드를 이용하여 Paste 구현.
        void PasteOnExecuted(object sender, ExecutedRoutedEventArgs args)
        {
            inkcanv.Paste();
        }
        // Delete를 "직접"구현 
        void DeleteOnExecuted(object sender, ExecutedRoutedEventArgs args)
        {
            foreach (Stroke strk in inkcanv.GetSelectedStrokes())
                inkcanv.Strokes.Remove(strk);
        }
        // Select All item: 모든 항목 (Stroke)선택.
        void SelectAllOnExecuted(object sender, ExecutedRoutedEventArgs args)
        {
            inkcanv.Select(inkcanv.Strokes);
        }
        // Format Selection item: StylusToolDialog 호츨(Invoke).
        void FormatOnClick(object sender, RoutedEventArgs args)
        {
            StylusToolDialog dlg = new StylusToolDialog();
            dlg.Owner = this;
            dlg.Title = "Format Selection";

            // 처음 선택된 선의 DrawingAttributes의 값을 얻어옴.
            StrokeCollection strokes = inkcanv.GetSelectedStrokes();

            if (strokes.Count > 0)
                dlg.DrawingAttributes = strokes[0].DrawingAttributes;
            else
                dlg.DrawingAttributes = inkcanv.DefaultDrawingAttributes;

            if ((bool)dlg.ShowDialog().GetValueOrDefault())
            {
                // 선택된 모든선의 DrawingAttributes를 설정.
                foreach (Stroke strk in strokes)
                    strk.DrawingAttributes = dlg.DrawingAttributes;
            }
        }
    }
}

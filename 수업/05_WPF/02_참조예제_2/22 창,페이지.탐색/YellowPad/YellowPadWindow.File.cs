//-----------------------------------------------------
// YellowPadWindow.File.cs (c) 2006 by Charles Petzold
//-----------------------------------------------------
using Microsoft.Win32;
using System;
using System.IO;
using System.Windows;
using System.Windows.Ink;
using System.Windows.Input;
using System.Windows.Markup;
using System.Windows.Media;

namespace Petzold.YellowPad
{
    public partial class YellowPadWindow : Window
    {
        // File New command: 모든 선을 지움 (초기화).
        void NewOnExecuted(object sender, ExecutedRoutedEventArgs args)
        {
            inkcanv.Strokes.Clear();
        }
        // File Open command: OpenFileDialog를 열고 ISF file을 로드.
        void OpenOnExecuted(object sender, ExecutedRoutedEventArgs args)
        {
            OpenFileDialog dlg = new OpenFileDialog();
            dlg.CheckFileExists = true;
            dlg.Filter = "Ink Serialized Format (*.isf)|*.isf|" +
                         "All files (*.*)|*.*";

            if ((bool)dlg.ShowDialog(this))
            {
                try
                {
                    FileStream file = new FileStream(dlg.FileName, 
                                            FileMode.Open, FileAccess.Read);
                    inkcanv.Strokes = new StrokeCollection(file);
                    file.Close();
                }
                catch (Exception exc)
                {
                    MessageBox.Show(exc.Message, Title);
                }
            }
        }
        // File Save command: SaveFileDialog를 띄운다.
        void SaveOnExecuted(object sender, ExecutedRoutedEventArgs args)
        {
            SaveFileDialog dlg = new SaveFileDialog();
            dlg.Filter = "Ink Serialized Format (*.isf)|*.isf|" +
                         "XAML Drawing File (*.xaml)|*.xaml|" +
                         "All files (*.*)|*.*";

            if ((bool)dlg.ShowDialog(this))
            {
                try
                {
                    FileStream file = new FileStream(dlg.FileName, 
                                            FileMode.Create, FileAccess.Write);

                    if (dlg.FilterIndex == 1 || dlg.FilterIndex == 3)
                        inkcanv.Strokes.Save(file);

                    else
                    {
                        // DrawingGroup object로 Stroke(선) 저장.
                        DrawingGroup drawgrp = new DrawingGroup();

                        foreach (Stroke strk in inkcanv.Strokes)
                        {
                            Color clr = strk.DrawingAttributes.Color;

                            if (strk.DrawingAttributes.IsHighlighter)
                                clr = Color.FromArgb(128, clr.R, clr.G, clr.B);

                            drawgrp.Children.Add(
                                new GeometryDrawing(
                                    new SolidColorBrush(clr),
                                    null, strk.GetGeometry()));
                        }
                        XamlWriter.Save(drawgrp, file);
                    }
                    file.Close();
                }
                catch (Exception exc)
                {
                    MessageBox.Show(exc.Message, Title);
                }
            }
        }
        // File Exit item: 프로그램 종료시 창을 같이 닫는다.
        void CloseOnExecuted(object sender, ExecutedRoutedEventArgs args)
        {
            Close();
        }
    }
}

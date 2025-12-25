//-----------------------------------------------------
// YellowPadWindow.Mode.cs (c) 2006 by Charles Petzold
//-----------------------------------------------------
using System;
using System.Windows;
using System.Windows.Controls;

namespace Petzold.YellowPad
{
    public partial class YellowPadWindow : Window
    {
        // Stylus-Mode : submenu 중 하나의 항목을 열려면 체크해야한다.
        void StylusModeOnOpened(object sender, RoutedEventArgs args)
        {
            MenuItem item = sender as MenuItem;

            foreach (MenuItem child in item.Items)
                child.IsChecked = inkcanv.EditingMode ==
                                    (InkCanvasEditingMode)child.Tag;
        }
        // 선택한 항목을 EditingMode property에 설정.
        void StylusModeOnClick(object sender, RoutedEventArgs args)
        {
            MenuItem item = sender as MenuItem;
            inkcanv.EditingMode = (InkCanvasEditingMode)item.Tag;
        }
        // Eraser-Mode : submenu 중 하나의 항목을 열려면 체크해야한다.
        void EraserModeOnOpened(object sender, RoutedEventArgs args)
        {
            MenuItem item = sender as MenuItem;

            foreach (MenuItem child in item.Items)
                child.IsChecked = inkcanv.EditingModeInverted ==
                                    (InkCanvasEditingMode)child.Tag;
        }
        // 선택한 항목으로부터 EditingModeInverted property설정.
        void EraserModeOnClick(object sender, RoutedEventArgs args)
        {
            MenuItem item = sender as MenuItem;
            inkcanv.EditingModeInverted = (InkCanvasEditingMode)item.Tag;
        }
    }
}

//-----------------------------------------------------
// YellowPadWindow.Help.cs (c) 2006 by Charles Petzold
//-----------------------------------------------------
using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;

namespace Petzold.YellowPad
{
    public partial class YellowPadWindow : Window
    {
        // Help command: YellowPadHelp¸¦ modeless dialog·Î ¶ç¿î´Ù.
        void HelpOnExecuted(object sender, ExecutedRoutedEventArgs args)
        {
            YellowPadHelp win = new YellowPadHelp();
            win.Owner = this;
            win.Show();
        }
        // About command: YellowPadAboutDialog¶ç¿î´Ù.
        void AboutOnClick(object sender, RoutedEventArgs args)
        {
            YellowPadAboutDialog dlg = new YellowPadAboutDialog();
            dlg.Owner = this;
            dlg.ShowDialog();
        }
    }
}

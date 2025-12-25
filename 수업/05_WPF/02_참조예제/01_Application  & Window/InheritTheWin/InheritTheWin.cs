//----------------------------------------------
// InheritTheWin.cs (c) 2005 by Charles Petzold
//----------------------------------------------
using System;
using System.Windows;
using System.Windows.Input;

namespace Petzold.InheritTheWin
{
    class InheritTheWin : Window
    {
        [STAThread]
        public static void Main()
        {
           
            Application app = new Application();
            app.Run(new InheritTheWin());
        }
        public InheritTheWin()
        {
            //Left = 500;
            //Top = 250;
           
           //Left = SystemParameters.PrimaryScreenHeight - Width;
           //Top = SystemParameters.PrimaryScreenHeight - Height;

            Left = (SystemParameters.WorkArea.Width - Width) / 2 +
                SystemParameters.WorkArea.Left;
            Top = (SystemParameters.WorkArea.Height - Height) / 2 +
                SystemParameters.WorkArea.Top;
            Title = "Inherit the Win";
        }
    }
}
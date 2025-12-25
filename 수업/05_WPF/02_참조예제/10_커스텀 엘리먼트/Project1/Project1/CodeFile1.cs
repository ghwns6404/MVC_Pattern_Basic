using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;

namespace Project1
{
    public class AAA : Window
    {
        [STAThread]
        public static void Main()
        {
            Application app = new Application();
            app.Run(new AAA());
        }
        public AAA()
        {
            UserControl1 btn = new UserControl1();
            btn.Text = "Click this button";
            btn.FontSize = 24;
            btn.HorizontalAlignment = HorizontalAlignment.Center;
            btn.VerticalAlignment = VerticalAlignment.Center;
            btn.Padding = new Thickness(5, 20, 5, 20);
          
            Content = btn;
        }
     
     
    }
}
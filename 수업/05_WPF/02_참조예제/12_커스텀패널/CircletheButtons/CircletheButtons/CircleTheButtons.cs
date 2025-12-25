using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;

namespace CircletheButtons
{
    class CircleTheButtons:Window
    {
        [STAThread]
        public static void Main()
        {
            Application app = new Application();
            app.Run(new CircleTheButtons());
        }

        public CircleTheButtons()
        {
            Title = "Circle the Buttons";

            RadialPanel pnl = new RadialPanel();
            pnl.Orientation = RadialPanelOrientation.ByWidth;
            pnl.ShowPieLines = true;
            Content = pnl;

            Random rand = new Random();

            for (int i = 0; i < 10; i++)
            {
                Button btn = new Button();
                btn.Content = "Button Number"+(i+1);
                //버튼안 텍스트 글꼴사이즈 랜덤..
                btn.FontSize+=rand.Next(10);
                pnl.Children.Add(btn);
            }
        }
    }
}

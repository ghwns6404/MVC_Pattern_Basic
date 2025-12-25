//-------------------------------------------------
// RenderTheGraphic.cs (c) 2006 by Charles Petzold
//-------------------------------------------------
using System;
using System.Windows;

namespace Petzold.RenderTheGraphic
{
    class RenderTheGraphic : Window
    {
        [STAThread]
        public static void Main()
        {
            Application app = new Application();
            app.Run(new RenderTheGraphic());
        }
        public RenderTheGraphic()
        {
            Title = "Render the Graphic";

            SimpleEllipse elips = new SimpleEllipse();

            //elips.HorizontalAlignment = System.Windows.HorizontalAlignment.Center;
            //elips.VerticalAlignment = System.Windows.VerticalAlignment.Center;

            //elips.Width = 26;
            //elips.Height = 26;

            Content = elips;
        }
    }
}

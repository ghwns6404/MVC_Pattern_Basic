using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Shapes;

namespace PaintOnCanvadClone
{
    class PaintOnCanvasClone : Window
    {
        [STAThread]
        public static void Main()
        {
            Application app = new Application();
            app.Run(new PaintOnCanvasClone());
        }

        public PaintOnCanvasClone()
        {
            Title = "Paint On Canvas Clone";

            CanvasClone canv = new CanvasClone();
            Content = canv;

            SolidColorBrush[] brushes = { Brushes.Pink, Brushes.Green, Brushes.Blue,
                                            Brushes.Yellow, Brushes.Gray };

            //브러쉬 개수만큼~~ 만듭니다잉~
            for (int i = 0; i < brushes.Length; i++)
            {
                Rectangle rect = new Rectangle();
                rect.Fill = brushes[i];
                rect.Width = 200;
                rect.Height = 200;
                canv.Children.Add(rect);

                CanvasClone.SetLeft(rect, 100 * (i + 1));
                CanvasClone.SetTop(rect, 100 * (i + 1));
            }
        }
    }
}

//----------------------------------------------------
// DrawButtonsOnBitmap.cs (c) 2006 by Charles Petzold
//----------------------------------------------------
using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Controls.Primitives;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;

namespace Petzold.DrawButtonsOnBitmap
{
    public class DrawButtonsOnBitmap : Window
    {
        [STAThread]
        public static void Main()
        {
            Application app = new Application();
            app.Run(new DrawButtonsOnBitmap());
        }
        public DrawButtonsOnBitmap()
        {
            Title = "Draw Buttons on Bitmap";

            // 버튼을 위치시킬 UniformGrid 생성
            UniformGrid unigrid = new UniformGrid();
            unigrid.Columns = 4;

            // UniformGrid 위에 32개의 토글버튼 객체를 생성
            for (int i = 0; i < 32; i++)
            {
                ToggleButton btn = new ToggleButton();
                btn.Width = 96;
                btn.Height = 24;
                btn.IsChecked = (i < 4 | i > 27) ^ (i % 4 == 0 | i % 4 == 3);
                unigrid.Children.Add(btn);
            }

            // 엘리먼트 또는 컨트롤을 프린터 페이지에 그릴때 반드시 Measure / Arrage 메소드를 호출해서 크기를 지정해줘야함.
            unigrid.Measure(new Size(Double.PositiveInfinity,
                                     Double.PositiveInfinity));

            Size szGrid = unigrid.DesiredSize;

            unigrid.Arrange(new Rect(new Point(0, 0), szGrid));

            // 이전 예제와 동일한 과정을 거침.
            RenderTargetBitmap renderbitmap =
                new RenderTargetBitmap((int)Math.Ceiling(szGrid.Width),
                                       (int)Math.Ceiling(szGrid.Height),
                                       96, 96, PixelFormats.Default);

           
            renderbitmap.Render(unigrid);

            
            Image img = new Image();
            img.Source = renderbitmap;

 
            Content = img;
        }
    }
}

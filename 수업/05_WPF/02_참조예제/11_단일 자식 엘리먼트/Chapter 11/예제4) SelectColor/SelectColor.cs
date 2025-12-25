//--------------------------------------------
// SelectColor.cs (c) 2006 by Charles Petzold
//--------------------------------------------
using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;

namespace Petzold.SelectColor
{
    public class SelectColor : Window
    {
        [STAThread]
        public static void Main()
        {
            Application app = new Application();
            app.Run(new SelectColor());
        }

        public SelectColor()
        {
            Title = "Select Color";
            SizeToContent = SizeToContent.WidthAndHeight;

            // StackPanel을 생성해 Window의 컨텐트로 지정
            StackPanel stack = new StackPanel();
            stack.Orientation = Orientation.Horizontal;         // 가로 정렬
            Content = stack;

            // 테스트용 버튼 생성
            Button btn = new Button();
            btn.Content = "Do-nothing button\nto test tabbing";
            btn.Margin = new Thickness(24);
            btn.HorizontalAlignment = HorizontalAlignment.Center;
            btn.VerticalAlignment = VerticalAlignment.Center;
            stack.Children.Add(btn);

            // ColorGrid 컨트롤 생성
            ColorGrid clrgrid = new ColorGrid();
            clrgrid.Margin = new Thickness(24);
            clrgrid.HorizontalAlignment = HorizontalAlignment.Center;
            clrgrid.VerticalAlignment = VerticalAlignment.Center;
            clrgrid.SelectedColorChanged += ColorGridOnSelectedColorChanged;
            stack.Children.Add(clrgrid);

            // 또 다른 테스트용 버튼 생성
            btn = new Button();
            btn.Content = "Do-nothing button\nto test tabbing";
            btn.Margin = new Thickness(24);
            btn.HorizontalAlignment = HorizontalAlignment.Center;
            btn.VerticalAlignment = VerticalAlignment.Center;
            stack.Children.Add(btn);
        }

        void ColorGridOnSelectedColorChanged(object sender, EventArgs args)
        {
            ColorGrid clrgrid = sender as ColorGrid;
            Background = new SolidColorBrush(clrgrid.SelectedColor);
        }
    }
}

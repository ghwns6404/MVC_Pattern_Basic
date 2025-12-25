using Petzold.SelectColorFromGrid;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Media;
using System.Windows.Input;
using System.Windows.Controls;
using System.Windows.Shapes;

namespace Petzold.SelectColorFromGrid
{
    class SelectColorFromMenuGrid:Window
    {
        [STAThread]
        public static void Main()
        {
            Application app = new Application();
            app.Run(new SelectColorFromMenuGrid());
        }
        public SelectColorFromMenuGrid()
        {
            Title = "Select Color from Menu Grid";

            //DockPanel   생성
            DockPanel dock = new DockPanel();
            Content = dock;

            //탑 메뉴가 될 Menu 생성
            Menu menu = new Menu();
            dock.Children.Add(menu);
            DockPanel.SetDock(menu, Dock.Top);

            //나머지 영역을 채울 TextBlock을 생성
            TextBlock text = new TextBlock();
            text.Text = Title;
            text.FontSize = 32;
            text.TextAlignment = TextAlignment.Center;
            dock.Children.Add(text);

            //메뉴에 항목을 추가
            MenuItem itemColor = new MenuItem();
            itemColor.Header = "_Color";
            menu.Items.Add(itemColor);

            MenuItem itemForeground = new MenuItem();
            itemForeground.Header = "_Foreground";
            menu.Items.Add(itemForeground);

            //윈도우 전정색과 바인딩되는 ColorGridBox를 생성
            ColorGridBox clrbox = new ColorGridBox();
            clrbox.SetBinding(ColorGridBox.SelectedValueProperty, "Foreground");
            clrbox.DataContext = this;
            itemForeground.Items.Add(clrbox);

            MenuItem itemBackground = new MenuItem();
            itemForeground.Header = "_Background";
            itemColor.Items.Add(itemBackground);


            //윈도우 배경색과 바인딩되는 ColorGridBox를 생성
            clrbox = new ColorGridBox();
            clrbox.SetBinding(ColorGridBox.SelectedValueProperty, "Background");
            clrbox.DataContext = this;
            itemBackground.Items.Add(clrbox);
        }
    }
}

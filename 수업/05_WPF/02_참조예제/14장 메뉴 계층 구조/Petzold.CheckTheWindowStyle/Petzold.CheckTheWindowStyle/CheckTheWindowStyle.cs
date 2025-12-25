using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Controls;

namespace Petzold.CheckTheWindowStyle
{
    class CheckTheWindowStyle:Window
    {
        MenuItem itemchecked;

        [STAThread]
        public static void Main()
        {
            Application app = new Application();
            app.Run(new CheckTheWindowStyle());
        }
        public CheckTheWindowStyle()
        {
            Title = "Check the window style";

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

            //*************************
            MenuItem itemstyle = new MenuItem();
            itemstyle.Header = "_Style";
            menu.Items.Add(itemstyle);

            itemstyle.Items.Add(CreateMenuItem("_No border or caption", WindowStyle.None));
            itemstyle.Items.Add(CreateMenuItem("_single-border Window", WindowStyle.SingleBorderWindow));
            itemstyle.Items.Add(CreateMenuItem("3_D-border Window", WindowStyle.ThreeDBorderWindow));
            itemstyle.Items.Add(CreateMenuItem("_Tool Window", WindowStyle.ToolWindow));
        }
        MenuItem CreateMenuItem(string str, WindowStyle style)
        {
            MenuItem item = new MenuItem();
            item.Header = str;
            item.Tag = style;
            item.IsChecked = (style == WindowStyle);
            item.Click += styleonclick;                         // 메뉴 생성과 동시에 같은 이벤트를 설정한다. 
            if (item.IsChecked)
                itemchecked = item;
            return item;
        }
        void styleonclick(object sender, RoutedEventArgs args)  // 동시에 같은 이벤트를 공유한다.
        {
            itemchecked.IsChecked = false;
            itemchecked = args.Source as MenuItem;
            itemchecked.IsChecked = true;                       // 이벤트 발생시 메뉴에 체크표시가 되도록 설정한다.
            WindowStyle = (WindowStyle)itemchecked.Tag;
        }
    }
}

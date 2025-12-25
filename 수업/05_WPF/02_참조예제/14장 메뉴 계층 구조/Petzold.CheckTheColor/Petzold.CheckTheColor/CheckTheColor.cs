using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Controls;
using System.Windows.Shapes;
using System.Reflection;

namespace Petzold.CheckTheColor
{
    class CheckTheColor:Window
    {
        TextBlock text;
        [STAThread]
        public static void Main()
        {
            Application app = new Application();
            app.Run(new CheckTheColor());
        }
        public CheckTheColor()
        {
            Title = "Check the color";

            DockPanel dock = new DockPanel();           // 생성자에서 독패널을 생성하는것을 주의하자! 
            Content = dock;

            //탑 메뉴가 될 Menu 생성

            Menu menu = new Menu();
            dock.Children.Add(menu);
            DockPanel.SetDock(menu, Dock.Top);

            // 나머지 영역을 채울 TextBlock을 생성
            text = new TextBlock();
            text.Text = Title;
            text.TextAlignment = TextAlignment.Center;
            text.FontSize = 32;
            text.Background = SystemColors.WindowBrush;
            text.Foreground = SystemColors.WindowTextBrush;
            dock.Children.Add(text);

            //메뉴 항목 생성
            MenuItem itemColor = new MenuItem();
            itemColor.Header = "_Color";
            menu.Items.Add(itemColor);

            MenuItem itemForeground = new MenuItem();
            itemForeground.Header = "_Foreground";
            itemForeground.SubmenuOpened += ForgroundOnOpened;
            itemColor.Items.Add(itemForeground);
            FillWithColors(itemForeground, ForegroundOnClick);

            MenuItem itemBackground = new MenuItem();
            itemBackground.Header = "_Background";
            itemBackground.SubmenuOpened += BackgroundOnOpened;
            itemColor.Items.Add(itemBackground);
            FillWithColors(itemBackground, BackgroundOnClick);
        }



        void FillWithColors(MenuItem itemParent, RoutedEventHandler handler)    // 컬러찾기
        {
            foreach (PropertyInfo prop in typeof(Colors).GetProperties())
            {
                Color clr = (Color)prop.GetValue(null, null);
                int iCount = 0;
                iCount += clr.R == 0 || clr.R == 255 ? 1 : 0;
                iCount += clr.G == 0 || clr.G == 255 ? 1 : 0;
                iCount += clr.B == 0 || clr.B == 255 ? 1 : 0;

                if (clr.A == 255 && iCount > 1)
                {
                    MenuItem item = new MenuItem();

                 //   Rectangle rect = new Rectangle();
                  //  rect.Fill = new SolidColorBrush(clr);
                  //  rect.Width = 2 * (rect.Height = 12);
                  //  item.Icon = rect;
                    
                    item.Header = "_" + prop.Name;
                    item.Tag = clr;
                    item.Click += handler;
                    itemParent.Items.Add(item);
                }
            }
        }
        void ForgroundOnOpened(object sender, RoutedEventArgs args)         // 드롭다운 이벤트 발생시.
        {
            MenuItem itemParent = sender as MenuItem;
            foreach (MenuItem item in itemParent.Items)
                item.IsChecked = ((text.Foreground as SolidColorBrush).Color == (Color)item.Tag);
        }
        void BackgroundOnOpened(object sender, RoutedEventArgs args)
        {
            MenuItem itemParent = sender as MenuItem;
            foreach (MenuItem item in itemParent.Items)
                item.IsChecked = ((text.Background as SolidColorBrush).Color == (Color)item.Tag);
        }
        void ForegroundOnClick(object sender, RoutedEventArgs args)         // 실제 프로퍼티 옵션 클릭시.
        {
            MenuItem item = sender as MenuItem;
            Color clr = (Color)item.Tag;
            text.Foreground = new SolidColorBrush(clr);
        }
        void BackgroundOnClick(object sender, RoutedEventArgs args)
        {
            MenuItem item = sender as MenuItem;
            Color clr = (Color)item.Tag;
            text.Background = new SolidColorBrush(clr);
        }
    }
}

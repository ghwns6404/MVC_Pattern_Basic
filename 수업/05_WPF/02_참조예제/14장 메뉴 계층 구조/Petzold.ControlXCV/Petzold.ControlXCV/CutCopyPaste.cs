using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using System.Windows.Input;
using System.Windows.Media.Imaging;


namespace Petzold.ControlXCV1
{
    class CutCopyPaste:Window
    {
        TextBlock text;
        protected MenuItem itemCut, itemCopy, itemPaste, itemDelete;

        //[STAThread]
        //public static void Main()
        //{
        //    Application app = new Application();
        //    app.Run(new CutCopyPaste());
        //}
        public CutCopyPaste()
        {
            Title = "Cut ,Copy,and Paste";
            //DockPanel   생성
            DockPanel dock = new DockPanel();
            Content = dock;

            //탑 메뉴가 될 Menu 생성
            Menu menu = new Menu();
            dock.Children.Add(menu);
            DockPanel.SetDock(menu, Dock.Top);

            //나머지 영역을 채울 TextBlock을 생성
            text = new TextBlock();
            text.Text = "Sample clipboard text";
            text.FontSize = 32;
            text.HorizontalAlignment = HorizontalAlignment.Center;
            text.VerticalAlignment = VerticalAlignment.Center;
            text.TextWrapping = TextWrapping.Wrap;
            dock.Children.Add(text);

            //Edit 메뉴 생성
            MenuItem itemEdit = new MenuItem();
            itemEdit.Header = "_Edit";
            itemEdit.SubmenuOpened += EditOnOpened;
            menu.Items.Add(itemEdit);

            //Edit 메뉴 항목 생성
            itemCut = new MenuItem();
            itemCut.Header = "Cu_t";
            itemCut.Click += CutOnClick;
            Image img = new Image();
            img.Source = new BitmapImage(new Uri("http://image-0.poco.cn/pic_center/img/099/0836c2fc677eb3400a0049b746a5b308_640.jpg"));
            itemCut.Icon = img;
            itemEdit.Items.Add(itemCut);


            itemCopy = new MenuItem();
            itemCopy.Header = "_Copy";
            itemCopy.Click += CopyOnClick;
            img = new Image();
            img.Source = new BitmapImage(new Uri("http://www.onegreen.net/QQ/UploadFiles/200806/200861551027277.gif"));
            itemCopy.Icon = img;
            itemEdit.Items.Add(itemCopy);


            itemPaste = new MenuItem();
            itemPaste.Header = "_Paste";
            itemPaste.Click += PasteOnClick;
            img = new Image();
            img.Source = new BitmapImage(new Uri("http://www.onegreen.net/QQ/UploadFiles/200806/200861551027422.gif"));
            itemPaste.Icon = img;
            itemEdit.Items.Add(itemPaste);


            itemDelete = new MenuItem();
            itemDelete.Header = "_Delete";
            itemDelete.Click += DeleteOnClick;
            img = new Image();
            img.Source = new BitmapImage(new Uri("http://www.onegreen.net/QQ/UploadFiles/200806/200861551027518.jpg"));
            itemDelete.Icon = img;
            itemEdit.Items.Add(itemDelete);
        }
        void EditOnOpened(object sender, RoutedEventArgs args)
        {
            itemCut.IsEnabled =
                itemCopy.IsEnabled = itemDelete.IsEnabled = text.Text != null && text.Text.Length > 0;
            itemPaste.IsEnabled = Clipboard.ContainsText();
        }
        protected void CutOnClick(object sender, RoutedEventArgs args)
        {
            CopyOnClick(sender, args);
            DeleteOnClick(sender,args);
        }
        protected void CopyOnClick(object sender, RoutedEventArgs args)
        {
            if (text.Text != null && text.Text.Length > 0)
                Clipboard.SetText(text.Text);
        }
        protected void PasteOnClick(object sender, RoutedEventArgs args)
        {
            if (Clipboard.ContainsText())
                text.Text = Clipboard.GetText();
        }
        protected void DeleteOnClick(object sender, RoutedEventArgs args)
        {
            text.Text = null;
        }
        
    }
}

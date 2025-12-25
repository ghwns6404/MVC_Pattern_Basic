using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Controls;
using System.Windows.Documents;

namespace Petzold.PopupContextMenu
{
    class PopupContextMenu:Window
    {
        ContextMenu menu;
        MenuItem itemBold, itemItalic;
        MenuItem[] itemDecor;
        Inline inlClicked;

        [STAThread]
        public static void Main()
        {
            Application app = new Application();
            app.Run(new PopupContextMenu());
        }
        public PopupContextMenu()
        {
            Title = "Popup Context Menu";

            //ContextMenu 생성
            menu = new ContextMenu();

            //"Bold" 항목 추가
            itemBold = new MenuItem();
            itemBold.Header = "Bold";
            menu.Items.Add(itemBold);

            //"Italic" 항목 추가
            itemItalic = new MenuItem();
            itemItalic.Header = "Italic";
            menu.Items.Add(itemItalic);

            //모든 TextDecorationLocation 멤버를 구함
            TextDecorationLocation[] locs=(TextDecorationLocation[])Enum.GetValues(typeof (TextDecorationLocation));

            //MenuItem 객체 배열을 생성를 구함
            itemDecor=new MenuItem[locs.Length];

            for (int i = 0; i < locs.Length; i++)
            {
                TextDecoration decor = new TextDecoration();
                decor.Location = locs[i];

                itemDecor[i] = new MenuItem();
                itemDecor[i].Header = locs[i].ToString();
                itemDecor[i].Tag = decor;
                menu.Items.Add(itemDecor[i]);
            }

            //전체 컨텍스트 메뉴를 핸들러 한 개로 처리
            menu.AddHandler(MenuItem.ClickEvent, new RoutedEventHandler(MenuOnClick));

            //윈도우 Content 를 위한 TextBlock을 생성
            TextBlock text = new TextBlock();
            text.FontSize = 32;
            text.HorizontalAlignment = HorizontalAlignment.Center;
            text.VerticalAlignment = VerticalAlignment.Center;
            Content = text;

            //문장을 단어로 분리
            string strQuote = "To be,or not to be,that is the question";
            string[] strWords = strQuote.Split();

            //각 단어로 Run 객체를 만들어서 TextBlock에 추가
            foreach (string str in strWords)
            {
                Run run = new Run(str);

                //TextDecorations이 실제로 컬렉션인지를 확인
                run.TextDecorations = new TextDecorationCollection();
                text.Inlines.Add(run);
                text.Inlines.Add(" ");
            }
        }
        protected override void OnMouseRightButtonUp(MouseButtonEventArgs e)
        {
            base.OnMouseRightButtonUp(e);

            if ((inlClicked = e.Source as Inline) != null)
            {
                //inLine 프로퍼티에 맞는 메뉴 항목인지 검사
                itemBold.IsChecked = (inlClicked.FontWeight == FontWeights.Bold);
                itemItalic.IsChecked = (inlClicked.FontStyle == FontStyles.Italic);

                foreach (MenuItem item in itemDecor)
                {
                    item.IsChecked = (inlClicked.TextDecorations.Contains(item.Tag as TextDecoration));
                }

                //컨텍스트 메뉴를 보여줌
                menu.IsOpen = true;
                e.Handled = true;
            }
        }

        void MenuOnClick(object sender, RoutedEventArgs args)
        {
            MenuItem item = args.Source as MenuItem;
            item.IsChecked ^= true;

            if (item == itemBold)
                inlClicked.FontWeight = (item.IsChecked ? FontWeights.Bold : FontWeights.Normal);
            else if (item == itemItalic)
                inlClicked.FontStyle = (item.IsChecked ? FontStyles.Italic : FontStyles.Normal);
            else
            {
                if (item.IsChecked)
                    inlClicked.TextDecorations.Add(item.Tag as TextDecoration);
                else
                    inlClicked.TextDecorations.Remove(item.Tag as TextDecoration);
            }
        }
    }
}

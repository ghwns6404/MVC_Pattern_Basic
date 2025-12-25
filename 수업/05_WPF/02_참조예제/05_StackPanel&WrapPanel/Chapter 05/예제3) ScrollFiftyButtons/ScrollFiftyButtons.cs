//---------------------------------------------------
// ScrollFiftyButtons.cs (c) 2006 by Charles Petzold
//---------------------------------------------------
using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;

namespace Petzold.ScrollFiftyButtons
{
    class ScrollFiftyButtons : Window
    {
        [STAThread]
        public static void Main()
        {
            Application app = new Application();
            app.Run(new ScrollFiftyButtons());
        }
        public ScrollFiftyButtons()
        {
            Title = "Scroll Fifty Buttons";
            SizeToContent = SizeToContent.Width;
            AddHandler(Button.ClickEvent, new RoutedEventHandler(ButtonOnClick));

            //ScrollViewer scroll = new ScrollViewer();  //스크롤 생성해서 컨텐츠에 넣음
            //Content = scroll;

            Viewbox view = new Viewbox();            //이 방법을 사용하면 창에 크기에 맞춰 버튼도 줄어 들기 떄문에 효율적이지는 않다.
            Content = view;

            StackPanel stack = new StackPanel();
            stack.Margin = new Thickness(5);
            //scroll.Content = stack;                    //스크롤 컨텐츠에 페널을 넣음

            view.Child = stack;


            //stack.AddHandler(Button.ClickEvent, new RoutedEventHandler(ButtonOnClick));       //버튼 이벤트 핸들러 따로 등록
            

            for (int i = 0; i < 50; i++)
            {
                Button btn = new Button();
                btn.Name = "Button" + (i + 1);
                btn.Content = btn.Name + " says 'Click me'";
                btn.Margin = new Thickness(5);

                stack.Children.Add(btn);
            }
        }

        void ButtonOnClick(object sender, RoutedEventArgs args)
        {
            Button btn = args.Source as Button;                         //args의 Source를 버튼으로 형변환 시키고

            if (btn != null)                                            //널을 지울경우 
                MessageBox.Show(btn.Name + " has been clicked",
                                "Button Click");
        }
    }
}

//------------------------------------------------
// StackTenButtons.cs (c) 2006 by Charles Petzold
//------------------------------------------------
using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;

namespace Petzold.StackTenButtons
{
    class StackTenButtons : Window
    {
        [STAThread]
        public static void Main()
        {
            Application app = new Application();
            app.Run(new StackTenButtons());
        }

        public StackTenButtons()
        {
            Title = "Stack Ten Buttons";

            StackPanel stack = new StackPanel();        //Defualt가 세로이다.
            Content = stack;
            Random rand = new Random();                 //렌덤 난수를 발생 시킴

            //원래 Content는 오직 하나의 객체를 담을 수 있다고 배웠는데
            //여러개의 객체를 담기 위해
            for (int i = 0; i < 10; i++)
            {
                Button btn = new Button();

                btn.Name = ((char)('A' + i)).ToString();     //아스키 코드를 이용하여 A부터 10개의 알파벳 발생함
                btn.FontSize += rand.Next(10);               //10개의 랜덤된 폰트 사이즈 난수 발생
                btn.Content = "Button " + btn.Name + " says 'Click me'";
                btn.Click += ButtonOnClick;

                

                //1. stack.Orientation = Orientation.Horizontal;              // Horizontal을 이용해서 Defualt값을 바꿀 수 있다.
                //2. btn.HorizontalAlignment = HorizontalAlignment.Center;    // 버튼 안에 글씨 크기로 따라 변경
                //3. stack.HorizontalAlignment = HorizontalAlignment.Center;  // 프로퍼티가 여전히 stretch이므로 가장큰 버튼 크기로 통일된다.
                //4-1  SizeToContent = SizeToContent.WidthAndHeight;            // 콘텐츠의 창을 panel 크기로 맞춘다.
                //4-2. btn.Margin = new Thickness(5);
                //4-2. stack.Margin = new Thickness(5);
                stack.Children.Add(btn);

                stack.Background = Brushes.Aquamarine;

                //string str = stack.Children[i].ToString();                  // 이런식으로 엘리먼트의 정보를 얻어올수도 있다.
            }




            stack.AddHandler(Button.ClickEvent, new RoutedEventHandler(ButtonOnClick));       //AddHandler메소드는 UIElement에 정의되어 있으므로 라우터를 통해 ButtonOnClick을 호출할 수 있다.

            //AddHandler(Button.ClickEvent, new RoutedEventHandler(ButtonOnClick));             //패널을 사용하지 않고 윈도우 자체에서 불러올 수도 있다.
        }
      


        void ButtonOnClick(object sender, RoutedEventArgs args)
        {
            Button btn = args.Source as Button;     //버튼의 정보 및 이벤트를 라우터에 포함

            MessageBox.Show("Button " + btn.Name + " has been clicked",
                            "Button Click");

            
        }
    }
}
//---------------------------------------------------
// StackThirtyButtons.cs (c) 2006 by Charles Petzold
//---------------------------------------------------
using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;

namespace Petzold.StackThirtyButtons
{
    class StackThirtyButtons : Window
    {
        [STAThread]
        public static void Main()
        {
            Application app = new Application();
            app.Run(new StackThirtyButtons());
        }
        public StackThirtyButtons()
        {
            Title = "Stack Thirty Buttons";
            SizeToContent = SizeToContent.WidthAndHeight;
            ResizeMode = ResizeMode.CanMinimize;

            AddHandler(Button.ClickEvent, new RoutedEventHandler(ButtonOnClick));   // 라우터를 통해 모든 버튼은 같은 Click 이벤트핸들러를 공유
            
            StackPanel stackMain = new StackPanel();                                // MainStackPanel을 하나 지정해 놓고
            stackMain.Orientation = Orientation.Horizontal;
            stackMain.Margin = new Thickness(5);
            Content = stackMain;

            for (int i = 0; i < 3; i++)
            {
                StackPanel stackChild = new StackPanel();
                stackMain.Children.Add(stackChild);                                 // 여기서 3개의 stackChild 패널을 Main에 담는다.

                for (int j = 0; j < 10; j++)
                {
                    Button btn = new Button();
                    btn.Content = "Button No. " + (10 * i + j + 1);
                    btn.Margin = new Thickness(5);
                    stackChild.Children.Add(btn);
                }
            }
        }
        void ButtonOnClick(object sender, RoutedEventArgs args)
        {
            MessageBox.Show("You clicked the button labeled " +
                            (args.Source as Button).Content);
        }
    }
}

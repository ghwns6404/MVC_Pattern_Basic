using System;
using System.Windows;
using System.Windows.Controls;  // button 추가하기위해

namespace Project1
{
    class Class1 : Window
    {
        [STAThread]
        public static void Main()
        {
            Application app = new Application();
            app.Run(new Class1());  // Run 시 Class1을 호출한다.
        }

        public Class1()
        {
            Title = "Dock Aound the Block";  // 타이틀설정

            DockPanel dock = new DockPanel();  // 도킹 객체 하나 생성
            Content = dock;                    // Content 프로퍼티에 넣는다.

            for (int i = 0; i < 5; i++)        // 총 5번 돌아가고
            {
                Button btn = new Button();    
                btn.Content = "Button No." + (i + 1);   // 버튼에 글자
                dock.Children.Add(btn);                 // 버튼을 도킹 차일드로 넣어준다.

               // btn.HorizontalAlignment = HorizontalAlignment.Center;  // 수평으로
               // btn.VerticalAlignment = VerticalAlignment.Center;      // 수직으로

                btn.SetValue(DockPanel.DockProperty, (Dock)(i % 4));  // 0 왼쪽 1 위 2 오른쪽 4아래

            }
            //dock.LastChildFill = false; 나머지공간을 채우는걸 허용 안한다.
        }

    }
}



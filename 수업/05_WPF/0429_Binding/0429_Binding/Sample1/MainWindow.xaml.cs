using System;
using System.Windows;
/*
UI-속성 ---------------- 데이터 속성
UI-속성 ---------------- 

1) UI속성 변경 --> [WPF 바인딩 시스템 감지] --> 데이터 속성 자동 변경
   - UI : WPF에서 제공한 UI

2) 데이터 속성 변경 --> [알림] --> [WPf 바인딩 시스템 감지 X] --> UI 속성 변경
    -데이터 : C# 클래스 객체
*/

namespace _0429_Binding
{
    /// <summary>
    /// MainWindow.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class MainWindow : Window
    {
        private Person per = new Person()
        {
            Name = "홍길동",
            Phone = "010-1111-1234",
            Age = 20
        };
        public MainWindow()
        {
            InitializeComponent();
            this.panel.DataContext = per; //1 개체와 peson을 바인딩하겠다
        }

        private void eraseButton_Click(object sender, RoutedEventArgs e)
        {
            MessageBox.Show("clear");
            per.Name = "";
            per.Phone = "";
            per.Age = null;
        }
    }
}

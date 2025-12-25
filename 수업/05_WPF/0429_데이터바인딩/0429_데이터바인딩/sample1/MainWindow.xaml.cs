using System;
using System.Windows;

/*
 UI-속성  ------------- 데이터 속성
 UI-속성  -------------

1) UI속성 변경 -->[WPF 바이딩 시스템 감지] --> 데이터 속성 자동 변경!
   - UI : WPF에서 제공한 UI

2) 데이터 속성 변경 - >[알림] -> [WPF 바인딩 시스템] ---> UI속성 변경
   - 데이터 : C# 클래스 객체   
*/ 


namespace _0429_데이터바인딩
{
    /// <summary>
    /// MainWindow.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class MainWindow : Window
    {
        private Person per = new Person()
        {
            Name    = "홍길동",
            Phone   = "010-1111-1234",
            Age     = 20
        };

        public MainWindow()
        {
            InitializeComponent();
            this.panel.DataContext = per;           //1)
        }

        private void eraseButton_Click(object sender, RoutedEventArgs e)
        {
            MessageBox.Show("clear");
            per.Name    = "";
            per.Phone   = "";
            per.Age     = null;
        }
    }
}

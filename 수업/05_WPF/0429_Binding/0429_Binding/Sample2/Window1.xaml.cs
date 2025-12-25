using System;
using System.Windows;

/*
예제 5-11) 데이터 아일랜드 : XML상의
*/

namespace _0429_Binding
{
    /// <summary>
    /// Window1.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class Window1 : Window
    {
        private Person per = null;
        public Window1()
        {
            InitializeComponent();
            per = (Person)FindResource("person"); //정적리소스 획득
        }

        private void eraseButton_Click(object sender, RoutedEventArgs e)
        {
            MessageBox.Show("clear");
            per.Name = "";
            per.Phone = "";
            per.Age = null;
        }

        private void eraseButton_Click_1(object sender, RoutedEventArgs e)
        {

        }
    }
}

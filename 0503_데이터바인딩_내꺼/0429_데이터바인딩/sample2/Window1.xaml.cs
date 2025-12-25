using System;
using System.Windows;
using System.Windows.Controls;
/*
예제 5-11] 데이터 아일랜드(정적 리소스) : XML상에 리소스 정의

예제 5-12] 명시적 바인딩.
<Label Height="25" Content="{Binding Path=Name, Source={StaticResource person2}}"/>

예제 5-13] UI요소의 속성간 바인딩

예제 5-14] 형식변환기 사용

예제 5-15,16] 예외처리, 사용자정의 예외처리 
*/

namespace _0429_데이터바인딩
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
            per = (Person)FindResource("person2");   //정적리소스 획득!

            //에러 핸들러 등록
            Validation.AddErrorHandler(age, age_ValidationError);
            Validation.AddErrorHandler(male, age_ValidationError);
        }

        private void age_ValidationError(object sender, ValidationErrorEventArgs e)
        {
            MessageBox.Show((string)e.Error.ErrorContent);
            //age.ToolTip = (string)e.Error.ErrorContent;
        }

        private void eraseButton_Click(object sender, RoutedEventArgs e)
        {
            per.Name    = "";
            per.Phone   = "";
            per.Age     = null;
            per.Male    = null;
        }
    }
}

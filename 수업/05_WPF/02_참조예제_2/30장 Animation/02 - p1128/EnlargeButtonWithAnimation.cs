//-----------------------------------------------------------
// EnlargeButtonWithAnimation.cs (c) 2006 by Charles Petzold
//-----------------------------------------------------------
using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Animation;

namespace Petzold.EnlargeButtonWithAnimation
{
    public class EnlargeButtonWithAnimation : Window
    {
        const double initFontSize = 12;
        const double maxFontSize = 48;
        Button btn;

        [STAThread]
        public static void Main()
        {
            Application app = new Application();
            app.Run(new EnlargeButtonWithAnimation());
        }
        public EnlargeButtonWithAnimation()
        {
            Title = "Enlarge Button with Animation";

            btn = new Button();
            btn.Content = "Expanding Button";
            btn.FontSize = initFontSize;
            btn.HorizontalAlignment = HorizontalAlignment.Center;
            btn.VerticalAlignment = VerticalAlignment.Center;
            btn.Click += ButtonOnClick;
            Content = btn;
        }
        void ButtonOnClick(object sender, RoutedEventArgs args)
        {
            //DoubleAnimation 객체생성
            DoubleAnimation anima = new DoubleAnimation();
            //Duration(지속시간)을 2초로 설정
            anima.Duration = new Duration(TimeSpan.FromSeconds(2));   
            //초기값부터
            anima.From = initFontSize;    
            //최종값까지 변화시킨다는 선언부분....
            anima.To = maxFontSize;
            anima.FillBehavior = FillBehavior.Stop;
            //BeginAnimation - UIElement에 속한 메소드
            //첫번째인자는 에니메이션을 적용할 의존 프로퍼티
            //두번째인자는 에니메이션을 시킬 DoulbeAnimaiton의 객체
            btn.BeginAnimation(Button.FontSizeProperty, anima);
            //btn.BeginAnimation(Button.FontSizeProperty, anima.CreateClock());

        }
    }
}

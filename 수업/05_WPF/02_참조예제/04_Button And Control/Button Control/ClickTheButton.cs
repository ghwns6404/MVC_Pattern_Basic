//-----------------------------------------------
// ClickTheButton.cs (c) 2006 by Charles Petzold
//-----------------------------------------------
using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Controls.Primitives;

namespace Petzold.ClickTheButton
{
    public class ClickTheButton : Window
    {
        Run runButton;                                                                           //버튼객체의 이벤트를 외부에서 이벤트를 주기위해 생성한 필드  

        [STAThread]
        public static void Main()
        {
            Application app = new Application();
            app.Run(new ClickTheButton());                                                   
        }
        public ClickTheButton()
        {
            Title = "Button Control";

            Button btn = new Button();

                                                 

            #region 버튼 속성 관련 추가된 부분


            //btn.Content = "_Click me, please!";
            //btn.Click += ButtonOnClick;  //버튼 컨트롤에 문자열 추가.."_"는 바로가기 키 설정
            //btn.Focus();                                                                       //버튼 컨트롤에 포커스(기본버튼) 설정
            //btn.IsDefault = true;                                                              //버튼에 포커스가 없어도 기본 버튼으로 설정하는 프로퍼티 Endter키 등에 반응
            //btn.IsCancel = true;                                                               //버튼에 Calcel 기능을 추가하는 프로퍼티 Esc 키 등에 반응하게 된다.

            //btn.Margin = new Thickness(96);                                                    //엘리먼트의 경겨 바깥에 여백을 부여 ... 여백공간에는 사용자 입력이 안된다.
            //btn.HorizontalContentAlignment = HorizontalAlignment.Left;                         //버튼 컨트롤에 할당된 문자열(객체)를 좌측 하단에 배치 (정렬)
            //btn.VerticalContentAlignment = VerticalAlignment.Bottom;

            //btn.Padding = new Thickness(96);                                                   //버튼 내부에 여백을 설정
            //btn.HorizontalAlignment = HorizontalAlignment.Center;                              //버튼의 크기를 컨텐츠에 맞게 스스로 조정 창의 위치는 중앙 이미 표시된 후라도 적용 
            //btn.VerticalAlignment = VerticalAlignment.Center;
            ////btn.Width = 96;                                                                    //버튼의 크기를 명시적으로 변경
            ////btn.Height = 96;

            //SizeToContent = SizeToContent.WidthAndHeight;                                     //버튼 크기에 맞춰서 창의 크기가 변함 버튼에 설정된 Margin 도 적용됨
            
            //btn.FontSize = 48;                                                                //할당된된 문자열을 변경 폰트 사이즈는 48 폰트종류는 Times New Roman
            //btn.FontFamily = new FontFamily("Times New Roman");

            //btn.Background = Brushes.AliceBlue;                                               //버튼의 색상 변경  
            //btn.Foreground = Brushes.DarkSalmon;
            //btn.BorderBrush = Brushes.Magenta;

            
            #endregion

            #region 버튼 이벤트 관련 추가된 부분

            #region 버튼 속성 
            
            //SizeToContent = SizeToContent.WidthAndHeight; 
            //btn.HorizontalAlignment = HorizontalAlignment.Center;                              //버튼의 크기를 컨텐츠에 맞게 스스로 조정 창의 위치는 중앙 이미 표시된 후라도 적용 
            //btn.VerticalAlignment = VerticalAlignment.Center;
           

            #endregion

            //btn.Content = runButton = new Run("Click me, please!");                             //버튼 컨트롤에 문자열 추가.."_"는 바로가기 키 설정

            //btn.Click += ButtonOnClick;                                                          //버튼 클릭 이벤트
            //btn.MouseEnter += ButtonOnMouseEnter;
            //btn.MouseLeave += ButtonOnMouseLeave;

            #endregion

            #region 버튼을 이미지로 변경

            Uri uri = new Uri("pack://application:,,/munch.png");                               //Uri 생성자
            BitmapImage bitmap = new BitmapImage(uri);                                          //Uri 생성자를 이용하여 비트맵 객체 생성

            Image img = new Image();                                                            //이미지 객체 생성
            img.Source = bitmap;                                                                //이미지 소스는 생성한 비트맵 객체
            img.Stretch = Stretch.None;                                                         //크기는 이미지 파일 크기로 ...

            btn.Content = img;                                                                  //버튼 Bottun.Content를 이미지 파일로 할당 

            #region 버튼속성
            btn.HorizontalAlignment = HorizontalAlignment.Center;                              //버튼의 크기를 컨텐츠에 맞게 스스로 조정 창의 위치는 중앙 이미 표시된 후라도 적용 
            btn.VerticalAlignment = VerticalAlignment.Center;
            #endregion
           
            #endregion

            //#region 버튼으로 클립보드 (붙여넣기) 제어
            //#region 버튼속성
            //btn.HorizontalAlignment = HorizontalAlignment.Center;
            //btn.VerticalAlignment = VerticalAlignment.Center;
            //#endregion

            //btn.Command = ApplicationCommands.Paste;                                            //Application 에 설정되 어 있는 Paste 를 버튼 커멘드로 바인딩 
            //btn.Content = ApplicationCommands.Paste.Text;                                       //Paste 의 이름문자열(붙여넣기)

            //CommandBindings.Add(new CommandBinding(ApplicationCommands.Paste,                   //Command에 이벤트 핸들러 바인드 
            //                    PasteOnExecute, PasteCanExecute));


            //#endregion

            #region 토글 버튼
            //ToggleButton Tbtn = new ToggleButton();                                             //토글 버튼 생성
            CheckBox Tbtn = new CheckBox();                                                   //체크 박스 생성
            Tbtn.Content = "Can _Resize";
            Tbtn.HorizontalAlignment = HorizontalAlignment.Center;
            Tbtn.VerticalAlignment = VerticalAlignment.Center;
            Tbtn.IsChecked = (ResizeMode == ResizeMode.CanResize);                              //IsChecked 이벤트 초기화
            Tbtn.Checked += ButtonOnChecked;                                                  //Button Checked 이벤트 메서드 할당
            Tbtn.Unchecked += ButtonOnChecked;                                                //Button UnChecked 이벤트 메서드 할당
            Content = Tbtn;
            #endregion

            //Content = btn;                                                                       //버튼 객체 자제를 Winodow.Content 프로퍼티 자체에 할당

        }
        void ButtonOnClick(object sender, RoutedEventArgs args)
        {
            MessageBox.Show("The button has been clicked and all is well.",Title);
        }

        #region 버튼 이벤트 메서드
        void ButtonOnMouseEnter(object sender, MouseEventArgs args)
        {
            runButton.Foreground = Brushes.Red;                                                // 마우스가 버튼 컨트롤 안으로 이동시 발생 
        }
        void ButtonOnMouseLeave(object sender, MouseEventArgs args)
        {
            runButton.Foreground = SystemColors.ControlTextBrush;                             // 마우스가 버튼 컨트롤 밖으로 이동시 발생 
        }
        #endregion

        #region 붙여넣기 이벤트 함수
        void PasteOnExecute(object sender, ExecutedRoutedEventArgs args)
        {
            Title = Clipboard.GetText();
        }
        void PasteCanExecute(object sender, CanExecuteRoutedEventArgs args)
        {
            args.CanExecute = Clipboard.ContainsText();
        }
        protected override void OnMouseDown(MouseButtonEventArgs args)
        {
            base.OnMouseDown(args);
            Title = "Button Control";
        }
        #endregion
        
      
        void ButtonOnChecked(object sender, RoutedEventArgs args)
        {
            ToggleButton Tbtn = sender as ToggleButton;
            ResizeMode =
                (bool)Tbtn.IsChecked ? ResizeMode.CanResize : ResizeMode.NoResize;
        }
    }
}

//AboutDialog.cs

//--------------------------------------
// AboutDialog.cs (c) 2011 by 소민호
//--------------------------------------

/*************************************************************************************
 클래스명 : AboutDialog
 필      드 : 없슴
 설      명 : '정보'대화 상자 윈도우 폼 클래스
                 해당 프로그램의 Assembly에서 속성에 대한 정보를 가져와
                 프로그램명, 버젼, 저작권 속성을 가져와서 출력한다.
**************************************************************************************/

using System;
using System.Diagnostics;
using System.Reflection;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Windows.Media;

namespace Petzold.NotepadClone
{
    public class AboutDialog : Window
    {
        public AboutDialog(Window owner)
        {
            //어셈블리에서 속성을 구함..  파일 제목 및 버젼 등등 프로그램의 정보를 표시하기 위함임.

            //속성에 접근하기 위해 executing assembly를 구함
            Assembly asmbly = Assembly.GetExecutingAssembly();

            //프로그램 이름을 위해 AssemblyTitle 속성을 구함
            AssemblyTitleAttribute title = (AssemblyTitleAttribute)asmbly.GetCustomAttributes(typeof(AssemblyTitleAttribute), false)[0];
            string strTitle = title.Title;

            //AssemblyFileVersion 속성을 구함(버젼 속성)

            AssemblyFileVersionAttribute version = (AssemblyFileVersionAttribute)asmbly.GetCustomAttributes(typeof(AssemblyFileVersionAttribute), false)[0];
            string strVersion = version.Version.Substring(0, 3);

            //AssemblyCopyRight 속성을 구함(저작권 관련 속성)
            AssemblyCopyrightAttribute copy = (AssemblyCopyrightAttribute)asmbly.GetCustomAttributes(typeof(AssemblyCopyrightAttribute), false)[0];
            string strCopyright = copy.Copyright;

            //대화상자의 표준 윈도우 프로퍼티

            Title = "About" + strTitle;                              //Dialog 타이틀바의 내용
            ShowInTaskbar = false;                                  //작업 표시줄에 표현 여부
            SizeToContent = SizeToContent.WidthAndHeight;           //창의 너비와 높이가 콘텐츠의 너비와 높이에 맞게 설정
            ResizeMode = ResizeMode.NoResize;                       //창크기 조정 여부..(조정 불가로 설정 - NoResize)
            Left = owner.Left + 96;
            Top = owner.Top + 96;                                   //다이얼로그 창을 만든 윈도우의 Left 96 Top 96 의 위치에 위치시킴

            //윈도우 Content를 위한 스택 패널 생성
            StackPanel stackMain = new StackPanel();
            Content = stackMain;                                    //해당 다이얼로그와 StackPanel을 연결함.

            //프로그램 명을 위한 텍스트 블록 생성
            TextBlock txtblk = new TextBlock();                      //텍스트 블록 객체 생성
            txtblk.Text = strTitle + "Version : " + strVersion + "\r\n" + strCopyright;         //텍스트 블록 내의 내용
            txtblk.FontFamily = new FontFamily("Times New Roman");   //폰트 설정
            txtblk.FontSize = 32;                                    //24포인트로 폰트 크기 설정
            txtblk.FontStyle = FontStyles.Italic;                    //폰트 스타일 설정
            txtblk.Margin = new Thickness(24);                       //여백 설정
            txtblk.HorizontalAlignment = HorizontalAlignment.Center; //텍스트 블록의 위치

            stackMain.Children.Add(txtblk);                          //StackPanel에 텍스트 블록 추가

            //웹사이트 링크를 위한 텍스트 블록 생성
            Run run = new Run("http://cyworld.com/1988_01_29");      //웹사이트 링크 주소 설정
            Hyperlink link = new Hyperlink(run);                     //하이퍼링크 설정
            link.Click += LinkOnClick;                               //링크를 클릭 했을 때 이벤트 발생!
            txtblk = new TextBlock(link);                            //하이퍼링크를 위한 텍스트 블록 생성
            txtblk.FontSize = 20;                                    //웹사이트 링크의 폰트 사이즈
            txtblk.HorizontalAlignment = HorizontalAlignment.Center; //웹사이트 링크의 위치

            stackMain.Children.Add(txtblk);                          //StackPanel에 추가함

            //OK버튼 생성
            Button btn = new Button();
            btn.Content = "OK";
            btn.IsDefault = true;                                   //기본 단추로 설정
            btn.IsCancel = true;                                    //취소 단추인지 설정.. ESC로도 활성화 가능!
            btn.HorizontalAlignment = HorizontalAlignment.Center;   //정렬은 중앙에
            btn.MinWidth = 48;                                      //최소 48픽셀의 가로 크기를 가진다.
            btn.Margin = new Thickness(24);                         //여백은 24포인트
            btn.Click += OkOnClick;                                 //OK를 클릭 했을 때 설정

            stackMain.Children.Add(btn);                            //스택 메인에 추가함

            btn.Focus();                                            //최초 버튼이 포커스를 가지고 있게 설정함.
        }

        //각각의 이벤트 핸들러

        //1) Link의 이벤트 핸들러
        void LinkOnClick(object sender, RoutedEventArgs args)
        {
            Process.Start("http://cyworld.com/1988_01_29");         //익스플로러를 이용해 해당 주소로 접속
        }

        //2) button의 이벤트 핸들러
        void OkOnClick(object sender, RoutedEventArgs args)
        {
            DialogResult = true;                                    //사용자가 버튼을 눌렀을 때 나오는 결과값이 있는지 여부를 얻는다.
                                                                    //여기서는 취소 버튼임으로 아무런 일도 일어나지 않는다.
        }

    }
}

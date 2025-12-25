/////// NotepadClone.Help.cs - Help메뉴 만드는 거 ///////

using System;

using System.Reflection;
using System.Windows;
using System.Windows.Controls;

namespace Petzold.NotepadClone
{
    public partial class NotepadClone
    {
        //--------------- 메뉴바에 Help 컬렉션 만들기 -----------------
        void AddHelpMenu(Menu menu)
        {
            //------------------------------------------------------------------
            // 메뉴의 컨트롤의 선택가능한 부분 만들기 (Help 항목 생성)
            MenuItem itemHelp = new MenuItem();

            // 만들 메뉴 이름 설정
            itemHelp.Header = "_Help";

            // 메뉴바의 Help부분을 클릭 했을때 서브 메뉴가 열린다.
            // ViewOnOpen => NotepadClone.View.cs
            itemHelp.SubmenuOpened += ViewOnOpen;   
            
            // 메뉴에 추가
            menu.Items.Add(itemHelp);
            //------------------------------------------------------------------

            //------------------------------------------------------------------
            // About 메뉴 아이템 객체 생성
            MenuItem itemAbout = new MenuItem();

            // 만들 서브 메뉴의 이름 설정
            itemAbout.Header = "_About" + strAppTitle + ". . .";

            // 서브메뉴 클릭 이벤트
            // AboutOnClick => NotepadClone.View.cs
            itemAbout.Click += AboutOnClick;                 

            // 위에서 만든 Help 메뉴 안에 서브메뉴 추가
            itemHelp.Items.Add(itemAbout);
            //------------------------------------------------------------------
        }

        //------- Help 메뉴의  About 클릭시 발생 하는 이벤트 --------
        void AboutOnClick(object sender, RoutedEventArgs args)
        {
            // 다이얼로그 객체 생성 (인자로 자신의 주소값을 넘김)
            // AboutDialog ==> 띄울 다이얼로그의 이름
            AboutDialog dlg = new AboutDialog(this);

            // 모달 대화상자 띄우기 (이 다음부터 AboutDialog 클래스 실행)
            dlg.ShowDialog();
        }
    }
}

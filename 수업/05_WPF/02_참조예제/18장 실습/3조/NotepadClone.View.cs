//NotepadClone.View.cs

//---------------------
//  NotepadClone.View.cs   2011 by 김영린
//---------------------


/*************************************************************************************
 클래스명 : NotepadClone
 필      드 : 없슴
 설      명 : _View Menu 와 그 안에 Status Bar 의 상태를 변경하는 _Status Bar 메뉴를 생성하는 
               AddViewMenu() 매서드 구현.  Status Bar메뉴의 체크에 관한 이벤트 핸들러 구현.          
**************************************************************************************/

using System;
using System.Windows;
using System.Windows.Controls;

namespace NotepadClone
{
    public partial class Notepadclone
    {
        MenuItem itemStatus;

        void AddViewMenu(Menu menu)
        {
            //탑 레벨 View  항목을 생성 ..
            MenuItem itemView = new MenuItem();         //메뉴 아이템 생성
            itemView.Header = "_View";                      //메뉴의 제목설정..
            itemView.SubmenuOpened += ViewOnOpen; //메뉴를 클릭하여 서브 매뉴가 열릴때의 이벤트 핸들러 등록.
            menu.Items.Add(itemView);                      // 메뉴에 생성한 아이템 추가.
            
            //View 메뉴의 상태바 항목을 생성.
            itemStatus = new MenuItem();                    //새로운 메뉴아이템 생성.
            itemStatus.Header = "_Status Bar";            // 제목설정..
            itemStatus.IsCheckable = true;                  // 체크가능하게 설정.
            itemStatus.Checked += StatusOnCheck;      // 체크가 됬을때와 풀릴때의 이밴트 핸들러 등록.
            itemStatus.Unchecked += StatusOnCheck;  // 
            itemView.Items.Add(itemStatus);              // View메뉴에 서브아이템으로 등록.
        }

        void ViewOnOpen(object sender, RoutedEventArgs args)
        {
            itemStatus.IsChecked = (status.Visibility == Visibility.Visible);       //스테이 터스바가 사용중이면 메뉴오픈할때 체크표시

        }


        void StatusOnCheck(object sender, RoutedEventArgs args)
        {
            MenuItem item = sender as MenuItem;
            status.Visibility = item.IsChecked ? Visibility.Visible : Visibility.Collapsed;    //Status Bar 매뉴 클릭시  체크가되면 스테이터스 바를 사용으로
                                                                                                                        // 체크가 해제되면 스테이터스바를 숨김( 접는다 ).
        }
    }

}
//--------------------------------------------------
// NotepadClone.Find.cs (c) 2006 by Charles Petzold
//--------------------------------------------------


/*************************************************************************************
 클래스명 : NotepadClone
 필      드 : string            - strFindWhat, strReplaceWith
             StringComparison   - strcomp
             Direction          - dirFind
 설      명 : 검색에 대한 메뉴(Find, Find Next, Replace) 추가 설정 및 기능 구현
              Find : 문자열 검색
              Find Next :다음으로 같은 문자열 검색
              Replace : 찾은 문자열을 새로 작성한 문자열로 수정
**************************************************************************************/

using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;

namespace Petzold.NotepadClone
{
    public partial class NotepadClone
    {
        string strFindWhat = "", strReplaceWith = "";                           // strFindWhat : 검색할 문자열 , strReplaceWith : 검색한 문자열
        StringComparison strcomp = StringComparison.OrdinalIgnoreCase;          // StringComparison 열거형 선언
        // OrdinalIgnoreCase : 대소문자를 무시하고 문자열 비교
        Direction dirFind = Direction.Down;                                     // 검색할 방향
        // =======================================================================================================================
        // 메뉴 초기화 및 설정들...
        void AddFindMenuItems(MenuItem itemEdit)
        {
            // Find 메뉴 항목 만들기...
            MenuItem itemFind = new MenuItem();                                 // itemFind 메뉴 추가
            itemFind.Header = "_Find...";                                       // Herder 설정
            itemFind.Command = ApplicationCommands.Find;                        // Command로 Find 설정
            itemEdit.Items.Add(itemFind);                                       // itemEdit에 itemFind 추가
            CommandBindings.Add(new CommandBinding(
                ApplicationCommands.Find, FindOnExecute, FindCanExecute));      // FindOnExecute, FindCanExecute 이벤트 연결
            // -------------------------------------------------------------------------------------------------------------------
            // 커스텀 RoutedUICommand는 Find Next 항목이 필요
            InputGestureCollection coll = new InputGestureCollection();         // RoutedUICommand에 추가할 coll 생성
            coll.Add(new KeyGesture(Key.F3));                                   // coll에 F3 단축키 등록
            RoutedUICommand commFindNext =
                new RoutedUICommand("Find _Next", "FindNext", GetType(), coll); // FindNext 설정(text, name, type, gestures)

            MenuItem itemNext = new MenuItem();                                 // itemNext 메뉴 추가
            itemNext.Command = commFindNext;                                    // 위에서 만든 commFindNext 연결
            itemEdit.Items.Add(itemNext);                                       // itemEdit에 itemNext 추가
            CommandBindings.Add(
                new CommandBinding(commFindNext, FindNextOnExecute,
                                                 FindNextCanExecute));          // FindNextOnExecute, FindNextCanExecute 이벤트 연결
            // -------------------------------------------------------------------------------------------------------------------
            MenuItem itemReplace = new MenuItem();                              // itemReplace 메뉴 추가
            itemReplace.Header = "_Replace...";                                 // Herder 설정
            itemReplace.Command = ApplicationCommands.Replace;                  // Command로 Replace 설정
            itemEdit.Items.Add(itemReplace);                                    // itemEdit에 itemReplace 추가
            CommandBindings.Add(new CommandBinding(
                ApplicationCommands.Replace, ReplaceOnExecute, FindCanExecute));// ReplaceOnExecute, FindCanExecute 이벤트 연결
        }
        // =======================================================================================================================
        // Find와 Replce를 위한 CanExecute 메소드
        void FindCanExecute(object sender, CanExecuteRoutedEventArgs args)
        {
            args.CanExecute = (txtbox.Text.Length > 0 && OwnedWindows.Count == 0);  // Find 메뉴 활성화 조건
        }
        void FindNextCanExecute(object sender, CanExecuteRoutedEventArgs args)
        {
            args.CanExecute = (txtbox.Text.Length > 0 && strFindWhat.Length > 0);   // Find Next 메뉴 활성화 조건
        }
        // =======================================================================================================================
        // Find 메뉴 항목에 대한 이벤트 핸들러
        void FindOnExecute(object sender, ExecutedRoutedEventArgs args)
        {
            // 대화상자 생성
            FindDialog dlg = new FindDialog(this);

            // 프로퍼티 초기화
            dlg.FindWhat = strFindWhat;                                         // 검색할 문자열
            dlg.MatchCase = strcomp == StringComparison.Ordinal;                // 서수 정렬 규칙을 사용하여 문자열을 비교
            dlg.Direction = dirFind;                                            // 검색 방향 설정

            // 이벤트 핸들러를 설정하고 대화상자를 출력
            dlg.FindNext += FindDialogOnFindNext;
            dlg.Show();
        }
        // Find Next 메뉴 항목에 대한 이벤트 핸들러
        // 문자열이 아직 없다면 F3 키로는 대화상자를 호출
        void FindNextOnExecute(object sender, ExecutedRoutedEventArgs args)
        {
            if (strFindWhat == null || strFindWhat.Length == 0)                 // 문자열이 있는지 비교
                FindOnExecute(sender, args);                                    // 없다면 FindOnExecute 대화상자 호출
            else
                FindNext();                                                     // 있다면 다음 문자열 검색
        }
        

        //=======================================================================================================================
        // Replace 메뉴 항목에 대한 이벤트 핸들러
        void ReplaceOnExecute(object sender, ExecutedRoutedEventArgs args)
        {
            ReplaceDialog dlg = new ReplaceDialog(this);

            //dlg 초기화
            dlg.FindWhat = strFindWhat;                     // 현재 strFindWhat변수에 들어있는 문자열을 dlg.FindWhat에 대입              
            dlg.ReplaceWith = strReplaceWith;               // 현재 strReplaceWith변수에 들어있는 문자열 dlg.ReplaceWith에 대입                
            dlg.MatchCase = strcomp == StringComparison.Ordinal; 
            dlg.Direction = dirFind;                        // 라디오 버튼에 따른 검색 방향 디폴트는 Down

            // Install 이벤트 핸들러
            dlg.FindNext += FindDialogOnFindNext;           // FindDialogOnFindNext 이벤트 연결
            dlg.Replace += ReplaceDialogOnReplace;          // ReplaceDialogOnReplace 이벤트 연결
            dlg.ReplaceAll += ReplaceDialogOnReplaceAll;    // ReplaceDialogOnReplaceAll 이벤트 연결

            dlg.Show();                                     // 모달리스 다이얼로그 생성
        }

        // Find/Replace 대화상자의 "Find Next" 버튼에 대한 이벤트 핸들러를 설정
        void FindDialogOnFindNext(object sender, EventArgs args)
        {
            FindReplaceDialog dlg = sender as FindReplaceDialog;    //sender를 FindReplaceDialog으로 형변환

            // 대화상자의 프로퍼티를 구함
            strFindWhat = dlg.FindWhat;                             //strFindWhat변수에 현재 dlg.FindWhat에 들어있는 문자열 대입
            strcomp = dlg.MatchCase ? StringComparison.Ordinal :    //문자열을 비교하여 결과값을 strcomp에 대입
                                StringComparison.OrdinalIgnoreCase;
            dirFind = dlg.Direction;                                // 라디오 버튼에 따른 검색 방향 디폴트는 Down   

            // 실제로 찾기 위해 FindNext를 호출
            FindNext();
        }

        // Replace 대화상자의 "Replace" 버튼에 대한 이벤트 핸들러를 설정
        void ReplaceDialogOnReplace(object sender, EventArgs args)
        {
            ReplaceDialog dlg = sender as ReplaceDialog;            //sender를 FindReplaceDialog으로 형변환

            // 대화상자의 프로퍼티를 구함
            strFindWhat = dlg.FindWhat;                             //strFindWhat변수에 현재 dlg.FindWhat에 들어있는 문자열 대입
            strReplaceWith = dlg.ReplaceWith;
            strcomp = dlg.MatchCase ? StringComparison.Ordinal :    //문자열을 비교하여 결과값을 strcomp에 대입
                                StringComparison.OrdinalIgnoreCase;

            if (strFindWhat.Equals(txtbox.SelectedText, strcomp))
                txtbox.SelectedText = strReplaceWith;               // 선택된 문자열을 strReplaceWith의 문자열로 바꿈

            FindNext();
        }

        // Replace 대화상자의 "Replace All" 버튼에 대한 핸들러 설정
        void ReplaceDialogOnReplaceAll(object sender, EventArgs args)
        {
            ReplaceDialog dlg = sender as ReplaceDialog;            //sender를 FindReplaceDialog으로 형변환
            string str = txtbox.Text;                               //str 변수에 texbox에있는 텍스트를 대입
            strFindWhat = dlg.FindWhat;                             //strFindWhat변수에 현재 dlg.FindWhat에 들어있는 문자열 대입
            strReplaceWith = dlg.ReplaceWith;                       //strReplaceWith변수에 현재 dlg.FindWhat에 들어있는 문자열 대입
            strcomp = dlg.MatchCase ? StringComparison.Ordinal :    //문자열을 비교하여 결과값을 strcomp에 대입
                                StringComparison.OrdinalIgnoreCase;
            int index = 0;

            while (index + strFindWhat.Length < str.Length)         //처음 선택된 택스트부터 텍스트 박스의 끝까지
            {
                index = str.IndexOf(strFindWhat, index, strcomp);   //??

                if (index != -1)
                {
                    str = str.Remove(index, strFindWhat.Length);    //원래있던 내용 지우고
                    str = str.Insert(index, strReplaceWith);        //새로운 내용 넣고
                    index += strReplaceWith.Length;                 //strReplaceWith의 길이만큼 인덱스에 더함
                }
                else
                    break;
            }
            txtbox.Text = str;                                      //바뀐 내용을 txtbox에 다시 대입
        }

        // 일반적인 FindNext 메소드
        void FindNext()
        {
            int indexStart, indexFind;

            //검색의 시작 위치와 검색 방향은 dirFind 변수에 의해 결정됨
            if (dirFind == Direction.Down)          //검색방향이 DOWN일때
            {
                indexStart = txtbox.SelectionStart + txtbox.SelectionLength;
                indexFind = txtbox.Text.IndexOf(strFindWhat, indexStart, strcomp);
            }
            else                                    //검색방향이 UP일때
            {
                indexStart = txtbox.SelectionStart;
                indexFind = txtbox.Text.LastIndexOf(strFindWhat, indexStart, strcomp);
            }

            //발견한 텍스트를 선택하고, 그렇지 않으면 메세지 박스 출력
            if (indexFind != -1)
            {
                txtbox.Select(indexFind, strFindWhat.Length);
                txtbox.Focus();
            }
            else
                MessageBox.Show("Cannot find \"" + strFindWhat + "\"", Title,
                                MessageBoxButton.OK, MessageBoxImage.Information);
        }
    }
}

//--------------------------------------------------
// NotepadClone.File.cs (c) 2011 by 이재찬, 이길호
//--------------------------------------------------

/*************************************************
 * 클래스 명 : NotepadClone
 * 필     드 : string strFilter(파일 열기, 저장에서 쓰이는 파일형식)
 * 설     명 : 해당 프로그램의 MainMenu 중 File의 하위메뉴인 새로만들기,열기, 저장, 
 *             다른이름으로 저장, 페이지설정, 인쇄, 종료 기능을 수행한다.
**************************************************/


using Microsoft.Win32;
using System;
using System.IO;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;
using System.Printing;

namespace Petzold.NotepadClone
{
    public partial class NotepadClone : Window
    {
        // Filter for File Open and Save dialog boxes.
        // 대화상자에서 파일 열기와 저장을 위한 필터
        protected string strFilter =
                        "Text Documents(*.txt)|*.txt|All Files(*.*)|*.*";

        // File 메뉴 초기화
        void AddFileMenu(Menu menu)
        {
            //============================================================
            // 탑 레벨 파일 항목 설정
            //============================================================
            MenuItem itemFile = new MenuItem();
            // 헤더 설정
            itemFile.Header = "_File";
            // Main 메뉴에 아이템(Top Menu) 담음
            menu.Items.Add(itemFile);

            // New 메뉴 항목 설정 및 Top메뉴 File에 추가
            MenuItem itemNew = new MenuItem();
            itemNew.Header = "_New";
            itemNew.Command = ApplicationCommands.New;
            itemFile.Items.Add(itemNew);
            CommandBindings.Add(
                new CommandBinding(ApplicationCommands.New, NewOnExecute));

            // Open  메뉴 항목 설정 및 Top메뉴 File에 추가
            MenuItem itemOpen = new MenuItem();
            itemOpen.Header = "_Open...";
            // Command 설정
            itemOpen.Command = ApplicationCommands.Open;
            itemFile.Items.Add(itemOpen);
            // CommandBing를 통한 이벤트 등록 Open과 OpenOnExecute함수 연결
            CommandBindings.Add(
                new CommandBinding(ApplicationCommands.Open, OpenOnExecute));

            // Save  메뉴 항목 설정 및 Top메뉴 File에 추가
            MenuItem itemSave = new MenuItem();
            itemSave.Header = "_Save";
            itemSave.Command = ApplicationCommands.Save;
            itemFile.Items.Add(itemSave);
            CommandBindings.Add(
                new CommandBinding(ApplicationCommands.Save, SaveOnExecute));

            // Save As  메뉴 항목 설정 및 Top메뉴 File에 추가
            MenuItem itemSaveAs = new MenuItem();
            itemSaveAs.Header = "Save _As...";
            itemSaveAs.Command = ApplicationCommands.SaveAs;
            itemFile.Items.Add(itemSaveAs);
            CommandBindings.Add(
                new CommandBinding(ApplicationCommands.SaveAs, SaveAsOnExecute));

            // 구분자(Separators) 삽입
            itemFile.Items.Add(new Separator());
            
            // PrintMenu를 추가하는 함수 호출(1조에서 알아서/ NotepadClone.Print.cs에 있음)
            AddPrintMenuItems(itemFile);
            itemFile.Items.Add(new Separator());

            // Exit  메뉴 항목 설정 및 Top메뉴 File에 추가
            MenuItem itemExit = new MenuItem();
            itemExit.Header = "E_xit";
            itemExit.Click += ExitOnClick;
            itemFile.Items.Add(itemExit);
        }

        // '새로만들기' 메뉴 함수
        // File New command: Start with empty TextBox.
        protected virtual void NewOnExecute(object sender, 
                                            ExecutedRoutedEventArgs args)
        {
            // 파일을 새로 만들지 물어보는 함수 호출
            if (!OkToTrash())
                return;

            txtbox.Text = "";       // 텍스트 내용 초기화
            strLoadedFile = null;   // 파일명(FullPath 포함) 초기화
            isFileDirty = false;    // 파일 저장확인을 위한 플래그를 false로 설정
            
            UpdateTitle();          // 타이틀바 수정하는 함수 호출(NotepadClone.cs에 있음)
        }

        // '열기' 메뉴 함수
        // File Open command: Display dialog box and load file.
        void OpenOnExecute(object sender, ExecutedRoutedEventArgs args)
        {
            if (!OkToTrash())
                return;


            OpenFileDialog dlg = new OpenFileDialog();      // 파일열기 다이얼로그 생성
            dlg.Filter = strFilter;                         // 파일형식 지정

            if ((bool)dlg.ShowDialog(this))                 // 파일열기 다이얼로그 보여주기
            {
                LoadFile(dlg.FileName);                     // 파일 불러오는 함수 호출
            }
        }

        // '저장' 메뉴 함수
        // File Save command: Possibly execute SaveAsExecute.
        void SaveOnExecute(object sender, ExecutedRoutedEventArgs args)
        {
            if (strLoadedFile == null || strLoadedFile.Length == 0)
                DisplaySaveDialog("");                      // 저장다이얼로그 띄우는 함수 호출
            else
                SaveFile(strLoadedFile);                    // 기존 존재하는 파일에 다시 저장하는 함수 호출
        }

        // '다른이름으로 저장' 메뉴 함수
        // File Save As command; display dialog box and save file.
        void SaveAsOnExecute(object sender, ExecutedRoutedEventArgs args)
        {
            DisplaySaveDialog(strLoadedFile);               // 저장다이얼르고 띄우는 함수 호출
        }

        // 파일저장 다이얼로그 띄우는 함수(저장할 시 true리턴, 취소할시 false리턴)
        // Display Save dialog box and return true if file is saved.
        bool DisplaySaveDialog(string strFileName)
        {
            SaveFileDialog dlg = new SaveFileDialog();      // 파일저장 다이얼로그 객체 생성
            dlg.Filter = strFilter;                         // 저장형식 지정
            dlg.FileName = strFileName;                     // 파일이름 지정

            if ((bool)dlg.ShowDialog(this))                 // 다이얼로그 띄우기(저장)누르면 true리턴)
            {
                SaveFile(dlg.FileName);                     // dlg에 설정된 FileName으로 Savefile호출
                return true;
            }
            return false;           // for OkToTrash.       // 취소 눌렀을 때
        }

        // '종료' 메뉴 함수
        // File Exit command: Just close the window.
        void ExitOnClick(object sender, RoutedEventArgs args)
        {
            // 프로그램 종료 함수 호출
            Close();
        }

        // 새로만들기나 열기 등에서 쓰이는 함수(텍스트 내용이 변경되면 isFileDirty이 false로 지정)
        // OkToTrash returns true if the TextBox contents need not be saved.
        bool OkToTrash()
        {
            if (!isFileDirty)   // 디렉토리가 존재하지 않는다면
                return true;    // true 리턴

            // 파일에 변화가 생겼을 시 MessageBox를 띄워서 MessageBoxResult에 리턴값 저장
            MessageBoxResult result =
                MessageBox.Show("The text in the file " + strLoadedFile + 
                                " has changed\n\n" +
                                "Do you want to save the changes?", 
                                strAppTitle,
                                MessageBoxButton.YesNoCancel,   // YesNoCancel 형태
                                MessageBoxImage.Question,       // image를 Questin 이미지
                                MessageBoxResult.Yes);          //  예(Y)에 Focus 맞춤

            // MessageBoxResult - Cancel / 취소 버튼을 눌렀을 때
            if (result == MessageBoxResult.Cancel)
                return false;

            // MessageBoxResult - No / 아니오 버튼을 눌렀을 때
            else if (result == MessageBoxResult.No)
                return true;

                // MessageBoxResult - Yes / 예 버튼을 눌렀을 때
            else // result == MessageBoxResult.Yes
            {
                // 파일저장
                if (strLoadedFile != null && strLoadedFile.Length > 0)
                    return SaveFile(strLoadedFile);


                return DisplaySaveDialog("");
            }
        }

        // 얻어온 파일명으로 Text에 추가하는 함수
        // LoadFile method possibly displays message box if error. 
        void LoadFile(string strFileName)
        {
            try
            {
                txtbox.Text = File.ReadAllText(strFileName);
            }
            catch (Exception exc) // 예외처리 발생시(*.*로 해서 파일 형식이 못읽을 경우..)
            {
                MessageBox.Show("Error on File Open: " + exc.Message, strAppTitle,
                                MessageBoxButton.OK, MessageBoxImage.Asterisk);
                return;
            }
            strLoadedFile = strFileName;    // 경로 등록
            UpdateTitle();                  // 타이틀 수정
            txtbox.SelectionStart = 0;      // TextBox 시작점 0으로 지정
            txtbox.SelectionLength = 0;     // TextBox문자열 길이 0으로 설정
            isFileDirty = false;            // 파일 저장 확인 플래그 false로 설정
        }
        
        // 얻어온 파일명으로 파일저장을 호출하는 함수
        // SaveFile method possibly displays message box if error.
        bool SaveFile(string strFileName)
        {
            try
            {
                // 해당 파일(경로)에 내용을 넣음
                File.WriteAllText(strFileName, txtbox.Text);
            }
            catch (Exception exc)
            {
                MessageBox.Show("Error on File Save" + exc.Message, strAppTitle,
                                MessageBoxButton.OK, MessageBoxImage.Asterisk);
                return false;
            }
            strLoadedFile = strFileName;
            UpdateTitle();
            isFileDirty = false;
            return true;
        }
    }
}

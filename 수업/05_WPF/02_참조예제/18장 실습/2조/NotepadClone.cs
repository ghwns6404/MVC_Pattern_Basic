//---------------------------------------------
// NotepadClone.cs (c) 2006 by Charles Petzold
//---------------------------------------------

/*************************************************
 * 클래스 명 : NotepadClone
 * 필     드 : string strAppTitle (프로그램의 타이틀바 이름)
 *             string strAppData  (설정 파일의 전체 파일 이름)
 *             NotepadCloneSettings settings (설정)
 *             bool isFileDirty = false (파일 저장 확인을 위한 플래그)
 *             Menu menu          (메뉴 컨트롤)         
               TextBox txtbox     (텍스트박스 컨트롤)       
               StatusBar status   (상태바 컨트롤)
 *             string strLoadedFile (불러온 파일의 전체 이름)
 *             StatusBarItem statLineCol (줄과 열 상태)
 * 설     명 : 파일의 저장 및 불러오기 및 상태바 표시
**************************************************/

using System;
using System.ComponentModel;
using System.IO;
using System.Reflection;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Controls.Primitives;
using System.Windows.Input;
using System.Windows.Media;

namespace Petzold.NotepadClone
{
    public partial class NotepadClone : Window
    {

        // settings = 이전에 실행되면서 저장해놓은 설정

        protected string strAppTitle;       // 프로그램의 타이틀바 이름
        protected string strAppData;        // 설정 파일의 전체 파일 이름
        protected NotepadCloneSettings settings;    // 설정
        protected bool isFileDirty = false; // 파일 저장 확인을 위한 플래그

        // 메인 윈도우에서 사용되는 컨트롤
        protected Menu menu;                // 메뉴 컨트롤
        protected TextBox txtbox;           // 텍스트박스 컨트롤
        protected StatusBar status;         // 상태바 컨트롤

        string strLoadedFile;               // 불러올 파일의 전체 이름
        StatusBarItem statLineCol;          // 줄과 열 상태

        [STAThread]
        public static void Main()
        {
            Application app = new Application();                // Application 생성
            app.ShutdownMode = ShutdownMode.OnMainWindowClose;
            // 경고창을 띄우기 전에 ShutdownMode를 ShutdownMode.OnMainWindowClose 으로 하면 경고창(혹은 MessageBox 류)의 확인 버튼을 눌러 
            // 경고창을 종료하면 별다른 Shutdown 명령을 내리지 않아도 해당 Application이 종료된다.
            app.Run(new NotepadClone());
        }
        public NotepadClone()
        {
            // 속성에 접금하기 위해 executing assembly를 구함
            Assembly asmbly = Assembly.GetExecutingAssembly();

            // strAppTitle(프로그램의 타이틀바 이름) 필드를 설정하기 위해 AssemblyTitle 속성을 구함
            // Assembly에 적용된 사용자 지정 특성의 배열을 검색해 Title에 넣는다
            AssemblyTitleAttribute title = (AssemblyTitleAttribute)asmbly.
                GetCustomAttributes(typeof(AssemblyTitleAttribute), false)[0];
            strAppTitle = title.Title;  

            // strAppData(전체파일이름) 파일 이름을 설정하기 위해 AssemblyProduct 속성을 구함
            AssemblyProductAttribute product = (AssemblyProductAttribute)asmbly.
                GetCustomAttributes(typeof(AssemblyProductAttribute), false)[0];
            // GetFolderPath로 지정된 열거형으로 식별되는 시스템 특수 폴더의 경로를 가져오고 Path.Combine으로 문자열 배열을 한 경로로 결합
            strAppData = Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData),
                "Petzold\\" + product.Product + "\\" + product.Product + ".Settings.xml");


            DockPanel dock = new DockPanel();   // DockPanel 생성
            Content = dock;                     // Content 프로퍼티에 DockPanel 객체 할당

            menu = new Menu();                  // 메뉴 생성
            dock.Children.Add(menu);            // DockPanel의 자식속성 값에 Menu 추가
            DockPanel.SetDock(menu, Dock.Top);  // Menu를 상단부분에 셋팅
 
            status = new StatusBar();           // 상태바 생성
            dock.Children.Add(status);          // DockPanel의 자식속성 값에 상태바 추가
            DockPanel.SetDock(status, Dock.Bottom); // 상태바를 하단부분에 셋팅

            statLineCol = new StatusBarItem();  // 선과 열을 보여주기 위해 StatusBarItem 생성
            statLineCol.HorizontalAlignment = HorizontalAlignment.Right;    // statLineCol의 위치를 부모요소에 할당된 레이아웃 공간의 오른쪽에 맞춘다
            status.Items.Add(statLineCol);      // 상태바에 statLineCol 추가
            DockPanel.SetDock(statLineCol, Dock.Right); // statLineCol을 오른쪽부분에 셋팅
 
           // 클라이언트 영역의 남은 부분을 채울 텍스트 박스 생성
            txtbox = new TextBox();             // 텍스트박스 생성
            txtbox.AcceptsReturn = true;        // 텍스트박스에 줄 바꿈 문자를 사용할 수 있다
            txtbox.AcceptsTab = true;           // Tab키를 누를 때 탭 순서에 따라 다음 컨트롤로 포커스를 이동하는 대신 해당 
                                                // 컨트롤에 탭 문자를 입력할지 여부를 나타내는 값 설정
            txtbox.VerticalScrollBarVisibility = ScrollBarVisibility.Auto;  // 세로 스크롤 막대를 자동으로 셋팅
            txtbox.HorizontalScrollBarVisibility = ScrollBarVisibility.Auto; // 가로 스크롤 막대를 자동으로 셋팅
            txtbox.TextChanged += TextBoxOnTextChanged; // 텍스트박스에 글자입력이 발생하면 TextBoxOnTextChanged 이벤트 호출
            txtbox.SelectionChanged += TextBoxOnSelectionChanged;   // 텍스트 선택 영역이 변경된 경우에 TextBoxOnSelectionChanged 이벤트 호출
            dock.Children.Add(txtbox);          // DockPanel의 자식속성 값에 텍스트박스 추가

            // 모든 탑 레벨 메뉴 항목 생성
            AddFileMenu(menu);          // in NotepadClone.File.cs
            AddEditMenu(menu);          // in NotepadClone.Edit.cs
            AddFormatMenu(menu);        // in NotepadClone.Format.cs
            AddViewMenu(menu);          // in NotepadClone.View.cs
            AddHelpMenu(menu);          // in NotepadClone.Help.cs

            // 이전에 실행되면서 저장해놓은 설정을 불러와서 settings에 셋팅
            settings = (NotepadCloneSettings) LoadSettings();

            // 저장된 설정을 Windowstate에 적용
            WindowState = settings.WindowState;
            
            if (settings.RestoreBounds != Rect.Empty)
            {
                Left = settings.RestoreBounds.Left;
                Top = settings.RestoreBounds.Top;
                Width = settings.RestoreBounds.Width;
                Height = settings.RestoreBounds.Height;
            }

            // Rect가 비어있지 않으면 RestoreBounds(창의 크기와 위치)를 셋팅
            if (settings.RestoreBounds != Rect.Empty)
            {
                Left = settings.RestoreBounds.Left;
                Top = settings.RestoreBounds.Top;
                Width = settings.RestoreBounds.Width;
                Height = settings.RestoreBounds.Height;
            }

            txtbox.TextWrapping = settings.TextWrapping;                // 텍스트박스에 settings의 줄바꿈 방식을 셋팅
            txtbox.FontFamily = new FontFamily(settings.FontFamily);    // 텍스트박스의 글꼴패밀리를 settings의 글꼴패밀리로 셋팅
            txtbox.FontStyle = (FontStyle)new FontStyleConverter().    //  텍스트박스의 글꼴스타일을 settings의 글꼴스타일로 셋팅
                                ConvertFromString(settings.FontStyle);
            txtbox.FontWeight = (FontWeight)new FontWeightConverter(). // 텍스트박스의 글꼴두께를 settings의 글꼴두께로 셋팅
                                ConvertFromString(settings.FontWeight);
            txtbox.FontStretch = (FontStretch)new FontStretchConverter(). // 텍스트박스의 글꼴늘이기를 settings의 글꼴늘이기로 셋팅   
                                ConvertFromString(settings.FontStretch);
            txtbox.FontSize = settings.FontSize;                            // 텍스트박스의 글꼴크기를 settings의 글꼴크기로 셋팅

            // Loaded 이벤트 핸들러 설정
            Loaded += WindowOnLoaded;

            // 텍스트박스에 포커스 설정
            txtbox.Focus();
        }
        // 생성자를 호출했을 때 설정을 불러들이는 메소드를 오버라이딩
        protected virtual object LoadSettings()
        {
            return NotepadCloneSettings.Load(typeof(NotepadCloneSettings),
                                             strAppData);
        }

        // Loaded 이벤트에 대한 이벤트 핸들러 : New 커맨드와 유사  
        // 명령 행에서 파일을 읽어 들일 수 있음
        void WindowOnLoaded(object sender, RoutedEventArgs args)
        {
            // 새로 실행
            ApplicationCommands.New.Execute(null, this);

            // 명령 행 인자를 구함
            string[] strArgs = Environment.GetCommandLineArgs();

            if (strArgs.Length > 1)         // strArgs의 길이가 1보다 작을 경우
            {
                if (File.Exists(strArgs[1]))    // strArgs[프로그램이름]의 지정된 파일이 있다면
                {
                    LoadFile(strArgs[1]);       // 파일 로드
                }
                else
                {
                    // 만약 파일이 없다면 메세지 박스를 띄워라
                    MessageBoxResult result = 
                        MessageBox.Show("Cannot find the " + 
                            Path.GetFileName(strArgs[1]) + " file.\r\n\r\n" +
                            "Do you want to create a new file?",
                            strAppTitle, MessageBoxButton.YesNoCancel,      // Yes , No , Cancle 창
                            MessageBoxImage.Question);                      // 물음표 아이콘 표시
                    
                    // 사용자가 Cancle을 클릭하면 윈도우를 종료
                    if (result == MessageBoxResult.Cancel)
                        Close();

                    
                    else if (result == MessageBoxResult.Yes)    // Yes이면 파일이라면
                    {
                        try
                        {
                            // 파일을 생성하고 종료
                            File.Create(strLoadedFile = strArgs[1]).Close();
                        }
                        catch (Exception exc)
                        {
                            MessageBox.Show("Error on File Creation: " + 
                                            exc.Message, strAppTitle,
                                MessageBoxButton.OK, MessageBoxImage.Asterisk);
                            return;
                        }
                        UpdateTitle();
                    }
                    // No이면 아무작업 안함
                }
            }
        }
        
        // OnClosing 이벤트
        protected override void OnClosing(CancelEventArgs args)
        {
            base.OnClosing(args);
            args.Cancel = !OkToTrash();             // 파일은 지워도 되지는 확인
            settings.RestoreBounds = RestoreBounds; // 원래창으로 복귀
        }
        // OnClosed 이벤트
        protected override void OnClosed(EventArgs args)
        {
            base.OnClosed(args);
            settings.WindowState = WindowState;                 // setting.WindowState에 최소화 또는 최대화하기 전의 창의 큭기와 위치 셋팅
            settings.TextWrapping = txtbox.TextWrapping;        // settings의 텍스트박슥의 줄바꿈 방식을 셋팅

            settings.FontFamily = txtbox.FontFamily.ToString(); // 텍스트박스의 폰트패밀리를 string 방식으로 settings의 폰트패밀리에 셋팅
            settings.FontStyle = 
                new FontStyleConverter().ConvertToString(txtbox.FontStyle); // 텍스트박스에 있는 글꼴 스타일을 string방식으로 settings의 글꼴스타일로 셋팅
            settings.FontWeight = 
                new FontWeightConverter().ConvertToString(txtbox.FontWeight); // 텍스트박스에 있는 글꼴두께를 string 방식으로 settings의 글꼴두께로 셋팅
            settings.FontStretch = 
                new FontStretchConverter().ConvertToString(txtbox.FontStretch); // 텍스트박스에 있는 글꼴늘이기를 string 방식으로 settings의 글꼴늘이기로 셋팅
            settings.FontSize = txtbox.FontSize;                // 텍스트박스에 있는 글꼴크기를 settings의 글꼴크기로 셋팅

            SaveSettings(); // 저장
        }

        // settings 객체의 Savs를 호출하기 위해 메소드를 오버라이딩
        protected virtual void SaveSettings()
        {
            // 전체파일이름을 저장
            settings.Save(strAppData);
        }
        
        // 파일명이나 Untitled를 출력
        protected void UpdateTitle()
        {
            if (strLoadedFile == null)                  //  불러올 파일이 없다면
                Title = "Untitled - " + strAppTitle;    // Title에 Untitled+strAppTitle 출력
            else
                Title = Path.GetFileName(strLoadedFile) + " - " + strAppTitle;  // 로드된 파일이름 +strAppTitle 얻어와서 출력
        }
        
        void TextBoxOnTextChanged(object sender, RoutedEventArgs args)
        {
            //텍스트박스의 텍스트가 변경되면 파일디렉터리를 true로 설정
            isFileDirty = true;
        }
       
        void TextBoxOnSelectionChanged(object sender, RoutedEventArgs args)
        {
            // selectionStart는 텍스트를 선택한경우 선택이 시작된 위치
            int iChar = txtbox.SelectionStart;
            // iLine 에 현재 커서의 위치에서 현재 라인위치를 가져와서 저장
            int iLine = txtbox.GetLineIndexFromCharacterIndex(iChar);

            // 에러 검사
            if (iLine == -1)
            {
                statLineCol.Content = "";
                return;
            }

            //현재 라인 위치의 가장 앞에 있는 텍스트의 위치를 알려주고
            int iCol = iChar - txtbox.GetCharacterIndexFromLineIndex(iLine);
            // 출력
            string str = String.Format("Line {0} Col {1}", iLine + 1, iCol + 1);

            if (txtbox.SelectionLength > 0)
            {
                iChar += txtbox.SelectionLength;
                iLine = txtbox.GetLineIndexFromCharacterIndex(iChar);
                iCol = iChar - txtbox.GetCharacterIndexFromLineIndex(iLine);
                str += String.Format(" - Line {0} Col {1}", iLine + 1, iCol + 1);
            }
            statLineCol.Content = str;
        }
    }
}

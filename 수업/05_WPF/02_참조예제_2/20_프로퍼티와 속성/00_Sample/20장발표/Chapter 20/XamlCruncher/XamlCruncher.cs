//---------------------------------------------
// XamlCruncher.cs (c) 2006 by Charles Petzold
//---------------------------------------------
using System;
using System.IO;                            // StringReader를 위한 지시자
using System.Text;                          // StringBuilder를 위한 지시자
using System.Windows;
using System.Windows.Controls;
using System.Windows.Controls.Primitives;   // StatusBarItem 위한 지시자 
using System.Windows.Input;
using System.Windows.Markup;                // XamlReader.Load 위한 지시자
using System.Windows.Media;
using System.Windows.Navigation;
using System.Windows.Threading;     // DispatcherUnhandledExceptionEventArgs 위한 지시자 
using System.Xml;                           // XmlTextReader 위한 지시자 

namespace Petzold.XamlCruncher
{
    class XamlCruncher : Petzold.NotepadClone.NotepadClone
    {
        Frame frameParent;          // XAML에 의해 생성된 객체를 보여주기 위한 프레임
        Window win;                 // XAML로부터 생성된 window
        StatusBarItem statusParse;  // 파싱에러나 성공사앹를 보여줌              파싱  :  원하는 부분 만을 뽑아서 가져오는 거예요.
        int tabspaces = 4;          // 탭키를 눌럿을때의 기본 간격 

        // 로드된 설정
        XamlCruncherSettings settingsXaml;

        // 메뉴 항목 관리 
        XamlOrientationMenuItem itemOrientation;
        bool isSuspendParsing = false;

        [STAThread]
        public new static void Main()
        {
            Application app = new Application();
            app.ShutdownMode = ShutdownMode.OnMainWindowClose;
            app.Run(new XamlCruncher());
        }
        // 파싱중단을 위한 프로퍼티 
        public bool IsSuspendParsing
        {
            set { isSuspendParsing = value; }
            get { return isSuspendParsing; }
        }
        // 생성자.
        public XamlCruncher()
        {
            // File open과 save 대화상자를 위한 필터 
            strFilter = "XAML Files(*.xaml)|*.xaml|All Files(*.*)|*.*";

            // DockPanel을 찾을후 그것에서 TextBox를 제거한다 
            DockPanel dock = txtbox.Parent as DockPanel;
            dock.Children.Remove(txtbox);

            // 3개의 열과 행으로 구성된 Gred를 생성하고 크기는 0으로 마춰춘다 
            //  Grid 열과 행으로 구성되는 유연한 표 영역을 정의합니다
            Grid grid = new Grid();
            dock.Children.Add(grid);

            for (int i = 0; i < 3; i++)
            {
                RowDefinition rowdef = new RowDefinition();
                rowdef.Height = new GridLength(0);
                grid.RowDefinitions.Add(rowdef);

                ColumnDefinition coldef = new ColumnDefinition();
                coldef.Width = new GridLength(0);
                grid.ColumnDefinitions.Add(coldef);
            }

            // 첫째열과 행의 크기를 100으로 초기화 시킨다 
            grid.RowDefinitions[0].Height = 
                        new GridLength(100, GridUnitType.Star);
            grid.ColumnDefinitions[0].Width = 
                        new GridLength(100, GridUnitType.Star);

            // 2개의 GridSplitter 컨트롤을  Grid에 추가 시칸다    GridSplitter 컨트롤은 Grid 사이즈를 자동 변경 할 수 있도록  해준다. 
            GridSplitter split = new GridSplitter();
            split.HorizontalAlignment = HorizontalAlignment.Stretch;
            split.VerticalAlignment = VerticalAlignment.Center;
            split.Height = 6;
            grid.Children.Add(split);
            Grid.SetRow(split, 1);
            Grid.SetColumn(split, 0);
            Grid.SetColumnSpan(split, 3);

            split = new GridSplitter();
            split.HorizontalAlignment = HorizontalAlignment.Center;
            split.VerticalAlignment = VerticalAlignment.Stretch;
            split.Width = 6;
            grid.Children.Add(split);
            Grid.SetRow(split, 0);
            Grid.SetColumn(split, 1);
            Grid.SetRowSpan(split, 3);

            // XAML 객체를 보여주기 위한 Frame 를 생성한다 
            frameParent = new Frame();
            frameParent.NavigationUIVisibility = NavigationUIVisibility.Hidden;
            grid.Children.Add(frameParent);

            // TextBox를 Grid에 붙힌다 
            txtbox.TextChanged += TextBoxOnTextChanged;
            grid.Children.Add(txtbox);

            // 로드된 설정을 XamlCruncherSettings로 형변환 시킨다 
            settingsXaml = (XamlCruncherSettings)settings;

            // Xaml메뉴를 최상위 매뉴로 삽입 시킨다 
            MenuItem itemXaml = new MenuItem();
            itemXaml.Header = "_Xaml";
            menu.Items.Insert(menu.Items.Count - 1, itemXaml);

            // XamlOrientationMenuItem을 생성하고 매뉴에 추가 시킨다 
            itemOrientation =
                new XamlOrientationMenuItem(grid, txtbox, frameParent);
            itemOrientation.Orientation = settingsXaml.Orientation;
            itemXaml.Items.Add(itemOrientation);

            //탭에 공백을 삽입하기 위한 메뉴 
            MenuItem itemTabs = new MenuItem();
            itemTabs.Header = "_탭간격설정...";
            itemTabs.Click += TabSpacesOnClick;
            itemXaml.Items.Add(itemTabs);

            //파싱 중단을 하기위한 메뉴 
            MenuItem itemNoParse = new MenuItem();
            itemNoParse.Header = "_Suspend Parsing";
            itemNoParse.IsCheckable = true;
            itemNoParse.SetBinding(MenuItem.IsCheckedProperty,
                                        "IsSuspendParsing");
            itemNoParse.DataContext = this;
            itemXaml.Items.Add(itemNoParse);

            // Reparse하기 위한 커맨드 
            InputGestureCollection collGest = new InputGestureCollection();
            collGest.Add(new KeyGesture(Key.F6));
            RoutedUICommand commReparse =
                new RoutedUICommand("_Reparse", "Reparse",
                                    GetType(), collGest);

            // Reparse를 위한 메뉴 
            MenuItem itemReparse = new MenuItem();
            itemReparse.Command = commReparse;
            itemXaml.Items.Add(itemReparse);

            // Reparse를 위한 커맨드 바인딩 
            CommandBindings.Add(new CommandBinding(commReparse,
                                ReparseOnExecuted));

            // 창을 보여주기위 한 커맨드 
            collGest = new InputGestureCollection();
            collGest.Add(new KeyGesture(Key.F7));
            RoutedUICommand commShowWin = 
                new RoutedUICommand("Show _Window", "ShowWindow",
                                    GetType(), collGest);

            // 창을 보여주기 위한 메뉴
            MenuItem itemShowWin = new MenuItem();
            itemShowWin.Command = commShowWin;
            itemXaml.Items.Add(itemShowWin);

            //창을 보여주기 위한 커맨드 바인딩 
            CommandBindings.Add(new CommandBinding(commShowWin,
                            ShowWindowOnExecuted, ShowWindowCanExecute));

            // Save as Startup Document 메뉴 
            MenuItem itemTemplate = new MenuItem();
            itemTemplate.Header = "Save as Startup _Document";
            itemTemplate.Click += NewStartupOnClick;
            itemXaml.Items.Add(itemTemplate);

            // Help 메뉴 상에 도움말을 삽입 
            MenuItem itemXamlHelp = new MenuItem();
            itemXamlHelp.Header = "_Help...";
            itemXamlHelp.Click += HelpOnClick;
            MenuItem itemHelp = (MenuItem)menu.Items[menu.Items.Count - 1];
            itemHelp.Items.Insert(0, itemXamlHelp);

            // 새로운 StatusBar(상태표시줄) 항목
            statusParse = new StatusBarItem();
            status.Items.Insert(0, statusParse);
            status.Visibility = Visibility.Visible;

           //여타 예외 상황들에 대한 핸들러를 설정
            //코드가 미치는 새로운 특징이나 변화를 표시한다 
            Dispatcher.UnhandledException += DispatcherOnUnhandledException;
        }
        // NewOnExecute 핸들러를 오버라이딩해 TextBox에 StartupDocument를 넣는다 
        protected override void NewOnExecute(object sender, 
                                             ExecutedRoutedEventArgs args)
        {
            base.NewOnExecute(sender, args);

            string str = ((XamlCruncherSettings)settings).StartupDocument;

            // 다음  Repalce 매소드가 /r/n을 오랍르게 삽힙하게 한다 
            str = str.Replace("\r\n", "\n");

            // 라인 피드를 캐리지 리턴/라인 피트로 바꾼다 
            //라인피드 : 모니터의 커서 위치나 프린터의 인쇄 위치를 한 줄 아래로 내리는 일 
            //캐리지 리턴은 커서의 위치를 현재 커서가 위치한 줄의 맨 처음으로 보내는 기능을 하고
            //라인 피드는 커서를 다음줄로 옮기는 역할을 한다 
            str = str.Replace("\n", "\r\n"); 
            txtbox.Text = str;
            isFileDirty = false;
        }
        // XamlCruncherSettings를 로드하게 LoadSettings를 오버라이딩 
        protected override object LoadSettings()
        {
            return XamlCruncherSettings.Load(typeof(XamlCruncherSettings), 
                                           strAppData);
        }
        // 메뉴에서 Orientation을 저장하는 OnClosed 오버라이딩 
        protected override void OnClosed(EventArgs args)
        {
            settingsXaml.Orientation = itemOrientation.Orientation; 
            base.OnClosed(args);
        }
        // XamlCruncherSettings 객체를 저장하는 SaveSettings를 오버라이딩 
        protected override void SaveSettings()
        {
            ((XamlCruncherSettings)settings).Save(strAppData);
        }
        //Tab Spaces 메뉴 핸들러  
        void TabSpacesOnClick(object sender, RoutedEventArgs args)
        {
            XamlTabSpacesDialog dlg = new XamlTabSpacesDialog();
            dlg.Owner = this;
            dlg.TabSpaces = settingsXaml.TabSpaces;

            if ((bool)dlg.ShowDialog().GetValueOrDefault())
            {
                settingsXaml.TabSpaces = dlg.TabSpaces;
            }
        }
        // Reparse 메뉴 핸들러 
        void ReparseOnExecuted(object sender, ExecutedRoutedEventArgs args)
        {
            Parse();
        }

        // Show Window 메뉴 핸들러 
        void ShowWindowCanExecute(object sender, CanExecuteRoutedEventArgs args)
        {
            args.CanExecute = (win != null);
        }
        void ShowWindowOnExecuted(object sender, ExecutedRoutedEventArgs args)
        {
            if (win != null)
                win.Show();
        }
        //  Save as New Startup Document 메뉴 핸들러 
        void NewStartupOnClick(object sender, RoutedEventArgs args)
        {
            ((XamlCruncherSettings)settings).StartupDocument = txtbox.Text;
        }
        // Help 메뉴 핸들러 
        void HelpOnClick(object sender, RoutedEventArgs args)
        {
            Uri uri = new Uri("pack://application:,,,/XamlCruncherHelp.xaml");
            Stream stream = Application.GetResourceStream(uri).Stream;//URI 객체를 먼저 얻고 Stream을 생성한다 

            Window win = new Window();//윈도 생성
            win.Title = "XAML Cruncher Help";//Title 설정
            win.Content = XamlReader.Load(stream);//리소스를 참조하는 Stream을 XamlReader.Load에 넘긴다 
            win.Show();

            //다른 방법 Frame 컨트롤을 생성하고 그것의 Source 프로퍼티에 직접 Uri 객체를 설정한다 
            //Frame frame = new Frame();
            //frame.Source = new Uri("pack://application:,,,/XamlCruncherHelp.xaml");
            //Window win = new Window();
            //win.Title = "XAML Cruncher Help";//Title 설정
            //win.Content = frame;
            //win.Show();
        }
        // OnPreviewKeyDown이 텝키를 공백으로 대체 
        protected override void OnPreviewKeyDown(KeyEventArgs args)
        {
            base.OnPreviewKeyDown(args);

            if (args.Source == txtbox && args.Key == Key.Tab)
            {
                string strInsert = new string(' ', tabspaces);
                int iChar = txtbox.SelectionStart;
                int iLine = txtbox.GetLineIndexFromCharacterIndex(iChar);

                if (iLine != -1)
                {
                    int iCol = iChar - txtbox.GetCharacterIndexFromLineIndex(iLine);
                    strInsert = new string(' ', 
                        settingsXaml.TabSpaces - iCol % settingsXaml.TabSpaces);
                }

                txtbox.SelectedText = strInsert;
                txtbox.CaretIndex = txtbox.SelectionStart + txtbox.SelectionLength;
                args.Handled = true;
            }
        }
        // TextBoxOnTextChanged가 XAML의 파싱을 수행 
        void TextBoxOnTextChanged(object sender, TextChangedEventArgs args)
        {
            if (IsSuspendParsing)
                txtbox.Foreground = SystemColors.WindowTextBrush;
            else
                Parse();
        }

        // 일반적인 Parse 메소드이며, Reparse 메뉴도 이메소드를 호출한다 
        void Parse()
        {
            StringReader strreader = new StringReader(txtbox.Text);
            XmlTextReader xmlreader = new XmlTextReader(strreader);

            try
            {
                object obj = XamlReader.Load(xmlreader);
                txtbox.Foreground = SystemColors.WindowTextBrush;

                if (obj is Window)
                {
                    win = obj as Window;
                    statusParse.Content = "Press F7 to display Window";
                }
                else
                {
                    win = null;
                    frameParent.Content = obj; 
                    statusParse.Content = "OK";
                }
            }
            catch (Exception exc)
            {
                txtbox.Foreground = Brushes.Red;
                statusParse.Content = exc.Message;
            }
        }
        //  XAML 객체가 예외 상황으로 빠질때 요구되는 UnhandledException 핸들러 
        void DispatcherOnUnhandledException(object sender, 
                                    DispatcherUnhandledExceptionEventArgs args)
        {
            statusParse.Content = "Unhandled Exception: " + args.Exception.Message;
            args.Handled = true;
        }
    }
}

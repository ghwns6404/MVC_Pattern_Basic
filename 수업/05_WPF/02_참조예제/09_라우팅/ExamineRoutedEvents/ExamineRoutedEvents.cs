//----------------------------------------------------
// ExamineRoutedEvents.cs (c) 2006 by Charles Petzold
//----------------------------------------------------
using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;

namespace Petzold.ExamineRoutedEvents
{
    public class ExamineRoutedEvents: Application
    {
        static readonly FontFamily fontfam = new FontFamily("Lucida Console");
        const string strFormat = "{0,-30} {1,-15} {2,-15} {3,-15}";
        StackPanel stackOutput;
        DateTime dtLast;

        [STAThread]
        public static void Main()
        {
            ExamineRoutedEvents app = new ExamineRoutedEvents();
            app.Run();
        }
        protected override void OnStartup(StartupEventArgs args)
        {
            base.OnStartup(args);

            // Window 생성.
            Window win = new Window();
            win.Title = "Examine Routed Events";

            // Grid를 생성하고 Window의 Content로 지정 .
            Grid grid = new Grid();
            win.Content = grid;

            // 3개의 행 생성.
            RowDefinition rowdef = new RowDefinition();
            rowdef.Height = GridLength.Auto;
            grid.RowDefinitions.Add(rowdef);

            rowdef = new RowDefinition();
            rowdef.Height = GridLength.Auto;
            grid.RowDefinitions.Add(rowdef);

            rowdef = new RowDefinition();
            rowdef.Height = new GridLength(100, GridUnitType.Star);
            grid.RowDefinitions.Add(rowdef);

            // Button을 생성하고 Grid에 추가.
            Button btn = new Button();
            btn.HorizontalAlignment = HorizontalAlignment.Center;
            btn.Margin = new Thickness(24);
            btn.Padding = new Thickness(24);
            grid.Children.Add(btn);

            // TextBlock을 생성하고 Button에 추가.
            TextBlock text = new TextBlock();
            text.FontSize = 24;
            text.Text = win.Title;
            btn.Content = text;

            // Scrollviewer 위에 제목 표시줄 생성.
            TextBlock textHeadings = new TextBlock();
            textHeadings.FontFamily = fontfam;
            textHeadings.Inlines.Add(new Underline(new Run(
                String.Format(strFormat, 
                "Routed Event", "sender", "Source", "OriginalSource"))));
            grid.Children.Add(textHeadings);
            Grid.SetRow(textHeadings, 1);

            // ScrollViewer 생성.
            ScrollViewer scroll = new ScrollViewer();
            grid.Children.Add(scroll);
            Grid.SetRow(scroll, 2);

            // 이벤트 출력을 위한 StackPanel을 생성.
            stackOutput = new StackPanel();
            scroll.Content = stackOutput;

            // 이벤트 핸들러 추가
            UIElement[] els = { win, grid, btn, text };

            foreach (UIElement el in els)
            {

                // Keyboard
                el.PreviewKeyDown += AllPurposeEventHandler;
                el.PreviewKeyUp += AllPurposeEventHandler;
                el.PreviewTextInput += AllPurposeEventHandler;
                el.KeyDown += AllPurposeEventHandler;
                el.KeyUp += AllPurposeEventHandler;
                el.TextInput += AllPurposeEventHandler;

                // Mouse
                el.MouseDown += AllPurposeEventHandler;
                el.MouseUp += AllPurposeEventHandler;
                el.PreviewMouseDown += AllPurposeEventHandler;
                el.PreviewMouseUp += AllPurposeEventHandler;

                // Stylus
                el.StylusDown += AllPurposeEventHandler;
                el.StylusUp += AllPurposeEventHandler;
                el.PreviewStylusDown += AllPurposeEventHandler;
                el.PreviewStylusUp += AllPurposeEventHandler;

               

                // Click
                el.AddHandler(Button.ClickEvent,
                    new RoutedEventHandler(AllPurposeEventHandler));
            }
            // Show the window.
            win.Show();
        }
        void AllPurposeEventHandler(object sender, RoutedEventArgs args)
        {
            // 시간이 지날 때 빈 줄 추가
            DateTime dtNow = DateTime.Now;
            if (dtNow - dtLast > TimeSpan.FromMilliseconds(100))
                stackOutput.Children.Add(new TextBlock(new Run(" ")));
            dtLast = dtNow;

            // 이벤트 정보를 출력
            TextBlock text = new TextBlock();
            text.FontFamily = fontfam;
            text.Text = String.Format(strFormat, 
                                      args.RoutedEvent.Name,
                                      TypeWithoutNamespace(sender),
                                      TypeWithoutNamespace(args.Source),
                                      TypeWithoutNamespace(args.OriginalSource));
            stackOutput.Children.Add(text);
            (stackOutput.Parent as ScrollViewer).ScrollToBottom();
        }
        string TypeWithoutNamespace(object obj)
        {
            string[] astr = obj.GetType().ToString().Split('.');
            return astr[astr.Length - 1];
        }
    }
}

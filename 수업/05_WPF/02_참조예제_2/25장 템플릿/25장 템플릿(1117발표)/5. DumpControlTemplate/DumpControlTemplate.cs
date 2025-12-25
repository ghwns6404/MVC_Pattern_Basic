//----------------------------------------------------
// DumpControlTemplate.cs (c) 2006 by Charles Petzold
//----------------------------------------------------
using System;
using System.Reflection;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Markup;
using System.Xml;

namespace Petzold.DumpControlTemplate
{
    public partial class DumpControlTemplate : Window
    {
        Control ctrl;

        [STAThread]
        public static void Main()
        {
            Application app = new Application();
            app.Run(new DumpControlTemplate());
        }
        public DumpControlTemplate()
        {
            InitializeComponent();
        }

        // Control 메뉴를 클릭했을 때 발생하는 이벤트 핸들러
        void ControlItemOnClick(object sender, RoutedEventArgs args)
        {
            // Remove any existing child from the first row of the Grid.
            for (int i = 0; i < grid.Children.Count; i++)
                if (Grid.GetRow(grid.Children[i]) == 0)
                {
                    grid.Children.Remove(grid.Children[i]);
                    break;
                }

            // TextBox를 비움
            txtbox.Text = "";

            // 클릭된 메뉴의 Control 클래스를 가져옴
            MenuItem item = args.Source as MenuItem;
            Type typ = (Type)item.Tag;

            // 선택된 타입의 객체를 생성하기 위한 준비
            ConstructorInfo info = typ.GetConstructor(System.Type.EmptyTypes);
            
            // 선택된 타입의 객체를 생성
            try
            {
                //Control ctrl;
                ctrl = (Control)info.Invoke(null);
            }
            catch (Exception exc)
            {
                MessageBox.Show(exc.Message, Title);
                return;
            }

            //생성된 객체를 그리드에 삽입
            //실패하면 생성된 객체는 Window
            try
            {
                grid.Children.Add(ctrl);
            }
            catch
            {
                if (ctrl is Window)
                    (ctrl as Window).Show();
                else
                    return;
            }
            Title = Title.Remove(Title.IndexOf('-')) + "- " + typ.Name;
        }
        // Dump메뉴가 열리면 항목을 활성화
        void DumpOnOpened(object sender, RoutedEventArgs args)
        {
            itemTemplate.IsEnabled = ctrl != null;
            itemItemsPanel.IsEnabled = ctrl != null && ctrl is ItemsControl;
        }
        // ControlTemplate 프로퍼티에 설정된 Template객체를 XAML로 내보냄
        void DumpTemplateOnClick(object sender, RoutedEventArgs args)
        {
            if (ctrl != null)
                Dump(ctrl.Template);
        }
        // ItemsPanel 프로퍼티에 설정된 ItemsPanelTemplate 객체를 XAML로 내보냄
        void DumpItemsPanelOnClick(object sender, RoutedEventArgs args)
        {
            if (ctrl != null && ctrl is ItemsControl)
                Dump((ctrl as ItemsControl).ItemsPanel);
        }
        // 탬플릿을 XMAL로 내보냄
        void Dump(FrameworkTemplate template)
        {
            if (template != null)
            {
                // XAML을 TextBox로 내보냄
                XmlWriterSettings settings = new XmlWriterSettings();
                settings.Indent = true; //들여쓰기
                settings.IndentChars = new string(' ', 4);
                settings.NewLineOnAttributes = true;    //새줄 쓰기

                StringBuilder strbuild = new StringBuilder();
                XmlWriter xmlwrite = XmlWriter.Create(strbuild, settings);

                try
                {
                    XamlWriter.Save(template, xmlwrite);
                    txtbox.Text = strbuild.ToString();
                }
                catch (Exception exc)
                {
                    txtbox.Text = exc.Message;
                }
            }
            else
                txtbox.Text = "no template";
        }
    }
}

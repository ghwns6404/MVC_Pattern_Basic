//-----------------------------------------------
// MeetTheDockers.cs (c) 2006 by Charles Petzold
//-----------------------------------------------
using System;
using System.Windows;
using System.Windows.Controls;          // 도구관련
using System.Windows.Controls.Primitives; //StatusBar, StatusBarItem

namespace Petzold.MeetTheDockers
{
    public class MeetTheDockers : Window
    {
        [STAThread]
        public static void Main()
        {
            Application app = new Application();
            app.Run(new MeetTheDockers());
        }
        public MeetTheDockers()
        {
            Title = "Meet the Dockers";

            //도킹을 생성
            DockPanel dock = new DockPanel();
            Content = dock;

            //메뉴 생성
            Menu menu = new Menu();

            //메뉴 아이템 삽입 
            MenuItem item = new MenuItem();
            item.Header = "Menu";
            menu.Items.Add(item);

            //메뉴 아이템 삽입2
            MenuItem item2 = new MenuItem();
            item2.Header = "About";
            menu.Items.Add(item2);

            //도킹을 설정한다 위로
            DockPanel.SetDock(menu, Dock.Top);
            dock.Children.Add(menu);  // 도킹에 추가

            // 툴바 생성
            ToolBar tool = new ToolBar();
            tool.Header = "Toolbar";

            // 툴바를 오른쪽으로 도킹
            DockPanel.SetDock(tool, Dock.Right);
            dock.Children.Add(tool);

            //상대바 생성
            StatusBar status = new StatusBar();

            //상태아이템 추가
            StatusBarItem statitem = new StatusBarItem();
            statitem.Content = "Status";
            status.Items.Add(statitem);
            
            //상태바 아래에 도킹
            DockPanel.SetDock(status, Dock.Bottom);
            dock.Children.Add(status);

            // 리스트박스 생성
            ListBox lstbox = new ListBox();
            lstbox.Items.Add("List Box Item");

            //리스트 박스 왼쪽에 추가
            DockPanel.SetDock(lstbox, Dock.Left);
            dock.Children.Add(lstbox);

            // 텍스트박스
            TextBox txtbox = new TextBox();
            txtbox.AcceptsReturn = true;  // 개행 추가(Enter허용)

            //도킹에 추가
            dock.Children.Add(txtbox);
            dock.LastChildFill = true;
            txtbox.Focus();
        }
    }
}

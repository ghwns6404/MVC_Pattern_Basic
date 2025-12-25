//----------------------------------------------
// BindTheButton.cs (c) 2006 by Charles Petzold
//----------------------------------------------
using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Controls.Primitives;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Data;

namespace Petzold.BindTheButton
{
    public class BindTheButton : Window
    {
        [STAThread]
        public static void Main()
        {
            Application app = new Application();
            app.Run(new BindTheButton());
        }
        public BindTheButton()
        {
            Title = "Bind theButton";

            ToggleButton btn = new ToggleButton();
            btn.Content = "Make _Topmost";
            btn.HorizontalAlignment = HorizontalAlignment.Center;
            btn.VerticalAlignment = VerticalAlignment.Center;

            //Binding bind = new Binding("Topmost");                    //Binding 클래스를 이용하여 Topmost 설정
            //bind.Source = this;                                       // 바인딩될 객체(소스)는 자기자신
            //btn.SetBinding(ToggleButton.IsCheckedProperty, bind);     //setBining 으로 토글 버튼의 IsCheckedPropert와 Binding 을 묶음
            
            btn.SetBinding(ToggleButton.IsCheckedProperty, "Topmost"); // 버튼에 바인딩을 세팅 Topmost 프로퍼티 사용..
            btn.DataContext = this;                                    // 바인딩이 될 객체 (Windows 창 ... 즉 자기 자신)
            Content = btn;

            ToolTip tip = new ToolTip();                               //툴팁 생성
            tip.Content = "Toggle the button on to make " +
                          "the window topmost on the desktop";
            btn.ToolTip = tip;                                         //Btb
        }
    }
}
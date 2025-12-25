//-------------------------------------------------
// ListNamedBrushes.cs (c) 2006 by Charles Petzold
//-------------------------------------------------
using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;

namespace Petzold.ListNamedBrushes
{
    public class ListNamedBrushes : Window
    {
        [STAThread]
        public static void Main()
        {
            Application app = new Application();
            app.Run(new ListNamedBrushes());
        }
        public ListNamedBrushes()
        {
            Title = "List Named Brushes";

            ListBox lstbox = new ListBox();
            lstbox.Width = 150;
            lstbox.Height = 150;
            Content = lstbox;

            //항목과 프로퍼티 패스를 수정...
            lstbox.ItemsSource = NamedBrush.All;
            lstbox.DisplayMemberPath = "Name";
            lstbox.SelectedValuePath = "Brush";             //<---리스트박스에 NamedBrush 객체를 넣으려면 brush로 설정...

            //selectedValue와 윈도우 배경색 바인딩..
            lstbox.SetBinding(ListBox.SelectedValueProperty, "Background");
            lstbox.DataContext = this;
        }
    }
}

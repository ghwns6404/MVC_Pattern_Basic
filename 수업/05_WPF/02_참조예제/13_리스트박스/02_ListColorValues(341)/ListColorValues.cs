using System;
using System.Reflection;
using System.Windows;
using System.Windows.Input;
using System.Windows.Controls;
using System.Windows.Media;

namespace Petzold.ListColorValues
{
    class ListColorValues : Window
    {
        [STAThread]
        public static void Main()
        {
            Application app = new Application();
            app.Run(new ListColorValues());
        }
        public ListColorValues()
        {
            Title = "List Color Values";

            ListBox lstbox = new ListBox();
            lstbox.Width = 150;
            lstbox.Height = 150;
            lstbox.SelectionChanged += ListBoxOnSelectionChanged;
            Content = lstbox;

            PropertyInfo[] props = typeof(Colors).GetProperties();
            //리스트 박스를 Color 값으로 체운다...
            foreach (PropertyInfo prop in props)
                lstbox.Items.Add(prop.GetValue(null, null));
        }
        void ListBoxOnSelectionChanged(object sender, 
                                       SelectionChangedEventArgs args)
        {
            ListBox lstbox = sender as ListBox;

            if (lstbox.SelectedIndex != -1)
            {
                Color clr = (Color)lstbox.SelectedItem;
                Background = new SolidColorBrush(clr);
            }
        }
    }
}

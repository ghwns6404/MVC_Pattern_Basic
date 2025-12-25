using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;

namespace _0428_WPF
{
    internal class MyWindow : Window 
    {
        private Button btn = new Button();
        public MyWindow()
        {
            btn.Content = "Click Me!";
            btn.Click += Btn_Click;

            //this.AddChild(btn);  
            this.Content = btn;
            this.Title = "My Window";   

            btn.Width = 100;
            btn.Height = 125;
        }

        private void Btn_Click(object sender, RoutedEventArgs e)
        {
            MessageBox.Show("Button clicked!");
        }
    }
}

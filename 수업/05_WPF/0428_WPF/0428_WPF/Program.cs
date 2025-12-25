using System;

using System.Windows;

namespace _0428_WPF
{
    internal class Program
    {
        [STAThread]
        static void Main(string[] args)
        {
            //MessageBox.Show("Hello, WPF!");    

            MyApp app = new MyApp();
            app.Run();

            MyWindow win = new MyWindow();  
            win.Show();
        }
    }
}

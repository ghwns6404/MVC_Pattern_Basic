using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.NetworkInformation;
using System.Text;
using System.Threading.Tasks;
using System.Windows;

namespace _0428_WPF
{
    internal class MyApp : Application
    {
        public MyApp()
        {
            this.Startup += MyApp_Startup;
        }

        private void MyApp_Startup(object sender, StartupEventArgs e)
        {
            Window window = new Window();
            window.Show();
        }   
    }
}

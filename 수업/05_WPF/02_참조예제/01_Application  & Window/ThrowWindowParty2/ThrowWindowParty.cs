//-------------------------------------------------
// ThreeWindowParty.cs (c) 2005 by Charles Petzold
//-------------------------------------------------
using System;
using System.Windows;
using System.Windows.Input;

namespace Petzold.ThrowWindowParty
{
    class ThrowWindowParty: Application
    {
        [STAThread]
        public static void Main()
        {
            ThrowWindowParty app = new ThrowWindowParty();
            //app.ShutdownMode = ShutdownMode.OnMainWindowClose;
            app.Run();
        }
        protected override void OnStartup(StartupEventArgs args)
        {
            Window winMain = new Window();
            winMain.Title = "Main Window";
            winMain.MouseDown += WindowOnMouseDown;
            winMain.Show();
           
            for (int i = 0; i < 2; i++)
            {
                Window win = new Window();
                win.Title = "Extra Window No. " + (i + 1);
                win.Show();
                win.Owner = winMain;
                //MainWindow = win;
                //win.ShowInTaskbar = false;
            }
            //ShutdownMode = ShutdownMode.OnMainWindowClose;
            //ShutdownMode = ShutdownMode.OnExplicitShutdown;
        }
        void WindowOnMouseDown(object sender, MouseButtonEventArgs args)
        {
            Window win = new Window();
            win.Title = "Modal Dialog Box";
            win.ShowDialog();
        }
    }
}


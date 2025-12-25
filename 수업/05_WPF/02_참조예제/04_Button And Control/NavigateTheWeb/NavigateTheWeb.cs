//-----------------------------------------------
// NavigateTheWeb.cs (c) 2006 by Charles Petzold
//-----------------------------------------------
using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;

namespace Petzold.NavigateTheWeb
{
    public class NavigateTheWeb : Window
    {
        Frame frm;

        [STAThread]
        public static void Main()
        {
            Application app = new Application();
            app.Run(new NavigateTheWeb());
        }
        public NavigateTheWeb()
        {
            Title = "Navigate the Web";

            frm = new Frame();              
            Content = frm;                  //Window 컨텐츠에게 Frame을 할당 

            Loaded += OnWindowLoaded;
        }
        void OnWindowLoaded(object sender, RoutedEventArgs args)
        {
            UriDialog dlg = new UriDialog(); //UriDialog에 생성
            dlg.Owner = this;                // 다이얼로그의 주인은 자기자신
            dlg.Text = "http://";           //Text 대입 
            dlg.ShowDialog();

            try
            {
                frm.Source = new Uri(dlg.Text);                  // ShowDialog가 반환한 값을 Frame Source 프로퍼티에 할당 
            }
            catch (Exception exc)
            {
                MessageBox.Show(exc.Message, Title);
            }
        }
    }
}

//------------------------------------------
// UriDialog.cs (c) 2006 by Charles Petzold
//------------------------------------------
using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;

namespace Petzold.NavigateTheWeb
{
    class UriDialog : Window
    {
        TextBox txtbox;

        public UriDialog()
        {
            Title = "Enter a URI";
            ShowInTaskbar = false;
            SizeToContent = SizeToContent.WidthAndHeight;
            WindowStyle = WindowStyle.ToolWindow;                               //툴 스타일 지정
            WindowStartupLocation = WindowStartupLocation.CenterOwner;          

            txtbox = new TextBox();
            txtbox.Margin = new Thickness(48);
            Content = txtbox;

            txtbox.Focus();
        }
        public string Text
        {
            set
            {
                txtbox.Text = value;                            
                txtbox.SelectionStart = txtbox.Text.Length;           // 커서위치를 텍스트의 마지막 ..
            }
            get
            {
                return txtbox.Text;
            }
        }
        protected override void OnKeyDown(KeyEventArgs args)
        {
            if (args.Key == Key.Enter)                              //OnKeyDown을 오버라이딩하여서 엔터키에 반응
                Close();
        }
    }
}

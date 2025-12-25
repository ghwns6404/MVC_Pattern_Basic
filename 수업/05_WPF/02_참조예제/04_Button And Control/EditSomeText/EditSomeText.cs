//---------------------------------------------
// EditSomeText.cs (c) 2006 by Charles Petzold
//---------------------------------------------
using System;
using System.ComponentModel;        // for CancelEventArgs
using System.IO;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;

namespace Petzold.EditSomeText
{
    class EditSomeText : Window
    {
        static string strFileName = Path.Combine(
            Environment.GetFolderPath(
                Environment.SpecialFolder.LocalApplicationData),                //캐리지 리턴을 이용하여 C:\Users\Administrator\AppData\Local\Petzold\EditSomeText폴더 안에 EditSomeText.txt파일 저장
                    "Petzold\\EditSomeText\\EditSomeText.txt");

        TextBox txtbox;

        [STAThread]
        public static void Main()
        {
            Application app = new Application();
            app.Run(new EditSomeText());
        }
        public EditSomeText()
        {
            Title = "Edit Some Text";

            // Create the text box.
            txtbox = new TextBox();
            txtbox.AcceptsReturn = true;                            //엔터를 입력받을수 있으며 멀티라인 설정
            txtbox.TextWrapping = TextWrapping.Wrap;
            txtbox.VerticalScrollBarVisibility = ScrollBarVisibility.Auto;      //오토 스크롤바 설정 
            txtbox.KeyDown += TextBoxOnKeyDown;
            Content = txtbox;

            // Load the text file.
            try
            {
                txtbox.Text = File.ReadAllText(strFileName);           //저장된 파일를 로드한다 .
            }
            catch
            {
            }

            // Set the text box caret and input focus.
            txtbox.CaretIndex = txtbox.Text.Length;                    //저장된 텍스트를 읽어오며 텍스트의 마지막에 커서를 둔다 
            txtbox.Focus();
        }
        protected override void OnClosing(CancelEventArgs args)
        {
            try
            {
                Directory.CreateDirectory(Path.GetDirectoryName(strFileName));  //지정된 폴더에 파일 저장
                File.WriteAllText(strFileName, txtbox.Text);
            }
            catch (Exception exc)
            {
                MessageBoxResult result = 
                    MessageBox.Show("File could not be saved: " + exc.Message + 
                                    "\nClose program anyway?", Title, 
                                    MessageBoxButton.YesNo, 
                                    MessageBoxImage.Exclamation);

                args.Cancel = (result == MessageBoxResult.No);
            }
        }
        void TextBoxOnKeyDown(object sender, KeyEventArgs args)
        {
            if (args.Key == Key.F5)
            {
                txtbox.SelectedText = DateTime.Now.ToString();     //F5키를 누르면 발생하는 이벤트 ..현재 날자를 출력한다.
                txtbox.CaretIndex = txtbox.SelectionStart + 
                                                txtbox.SelectionLength;
            }
        }
    }
}

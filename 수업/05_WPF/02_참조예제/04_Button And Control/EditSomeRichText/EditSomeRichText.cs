//--------------------------------------------------
// EditSomeRichText.cs (c) 2006 by Charles Petzold
//--------------------------------------------------
using Microsoft.Win32;     //파일 다이얼로그를 위해 추가
using System;
using System.IO;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;

namespace Petzold.EditSomeRichText
{
    public class EditSomeRichText : Window
    {
        RichTextBox txtbox;
        string strFilter = 
            "Document Files(*.xaml)|*.xaml|All files (*.*)|*.*";

        [STAThread]
        public static void Main()
        {
            Application app = new Application();
            app.Run(new EditSomeRichText());
        }
        public EditSomeRichText()
        {
            Title = "Edit Some Rich Text";

            txtbox = new RichTextBox();
            txtbox.VerticalScrollBarVisibility = ScrollBarVisibility.Auto;
            Content = txtbox;

            txtbox.Focus();
        }
        protected override void OnPreviewTextInput(TextCompositionEventArgs args)
        {
            if (args.ControlText.Length > 0 && args.ControlText[0] == '\x0F')  //window에서 먼저 발생된 Clrt + O 의 값을 감지
            {
                OpenFileDialog dlg = new OpenFileDialog();                      //오픈 파일 다이얼로그 초기화 
                dlg.CheckFileExists = true;                                     
                dlg.Filter = strFilter;

                if ((bool)dlg.ShowDialog(this))
                {
                    FlowDocument flow = txtbox.Document;                    //서식이 있는 Text를 Rich 
                    TextRange range = new TextRange(flow.ContentStart,      //불러온 컨텐츠의 크기 만큼 텍스트 범위 지정
                                                    flow.ContentEnd);
                    Stream strm = null;

                    try
                    {
                        strm = new FileStream(dlg.FileName, FileMode.Open);  // 받아온 파일stream 을 저장할 스트림에 넣는다 
                        range.Load(strm, DataFormats.Xaml);                  // 파일을 불러온다 
                    }
                    catch (Exception exc)
                    {
                        MessageBox.Show(exc.Message, Title);
                    }
                    finally
                    {
                        if (strm != null)
                            strm.Close();
                    }
                }

                args.Handled = true;
            }
            if (args.ControlText.Length > 0 && args.ControlText[0] == '\x13')
            {
                SaveFileDialog dlg = new SaveFileDialog();
                dlg.Filter = strFilter;

                if ((bool)dlg.ShowDialog(this))
                {
                    FlowDocument flow = txtbox.Document;
                    TextRange range = new TextRange(flow.ContentStart, 
                                                    flow.ContentEnd);
                    Stream strm = null;

                    try
                    {
                        strm = new FileStream(dlg.FileName, FileMode.Create);
                        range.Save(strm, DataFormats.Xaml);
                    }
                    catch (Exception exc)
                    {
                        MessageBox.Show(exc.Message, Title);
                    }
                    finally
                    {
                        if (strm != null)
                            strm.Close();
                    }
                }
                args.Handled = true;
            }
            base.OnPreviewTextInput(args);
        }
    }
}
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace CommonDialog
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void btn_FileOpen_Click(object sender, EventArgs e)
        {
            OpenFileDialog dlg = new OpenFileDialog();
            dlg.Filter = "RichText files (*.rtf)|*.rtf|All files (*.*)|*.*";
            dlg.InitialDirectory = "c:\\";
            dlg.Title = "파일 열기 대화상자 예제";

            if(dlg.ShowDialog() == DialogResult.OK)
            {
                this.textBox1.Text = dlg.FileName + " [[파일열기]]";
                this.richTextBox1.LoadFile(dlg.FileName, RichTextBoxStreamType.RichText);
            }
        }

        private void btn_FileSave_Click(object sender, EventArgs e)
        {
            SaveFileDialog dlg = new SaveFileDialog();
            
            dlg.Title = "파일 저장 대화상자 예제";
            dlg.CreatePrompt = true;
            dlg.OverwritePrompt = true;
            dlg.FileName = "default";
            dlg.DefaultExt = "rtf";
            dlg.InitialDirectory = "c:\\";
            dlg.Filter = "RichText files (*.rtf)|*.rtf";

            System.IO.MemoryStream memstream = new System.IO.MemoryStream();
            this.richTextBox1.SaveFile(memstream, RichTextBoxStreamType.RichText);

            if (dlg.ShowDialog() == DialogResult.OK)
            {
                try
                {
                    System.IO.Stream fs = dlg.OpenFile();
                    memstream.Position = 0;
                    memstream.WriteTo(fs);
                    fs.Close();
                    this.textBox1.Text = dlg.FileName + " [[파일저장]]";
                }
                catch(Exception ex)
                {
                    MessageBox.Show(ex.Message);
                }
            }

        }

        private void btn_Font_Click(object sender, EventArgs e)
        {   
            /*
            FontDialog dlg = new FontDialog();            
            if(dlg.ShowDialog() == DialogResult.OK)
            {
                this.richTextBox1.SelectionFont = dlg.Font;
                this.textBox1.Text = "글꼴을 " + dlg.Font + " 로 변경!!!";
            }
            */
            FontDialog dlg = new FontDialog();
            dlg.ShowApply = true;
            dlg.ShowColor = true;            

            if (dlg.ShowDialog() == DialogResult.OK)
            {
                this.richTextBox1.SelectionFont = dlg.Font;
                this.textBox1.Text = "글꼴을 " + dlg.Font + " 로 변경!!!";
            }
        }

        private void btn_Color_Click(object sender, EventArgs e)
        {
            ColorDialog dlg = new ColorDialog();
            if(dlg.ShowDialog() == DialogResult.OK)
            {
                this.richTextBox1.SelectionColor = dlg.Color;
                this.textBox1.Text = "색상을 " + dlg.Color + " 로 변경!!!";
            }
        }

        private void btn_FolderBrowser_Click(object sender, EventArgs e)
        {
            FolderBrowserDialog dlg = new FolderBrowserDialog();
            dlg.Description = "디렉토리를 지정하세요!";
            dlg.ShowNewFolderButton = true;
            dlg.RootFolder = Environment.SpecialFolder.MyComputer;            

            if(dlg.ShowDialog() == DialogResult.OK)
            {
                this.textBox1.Text = "선택한 폴더: " + dlg.SelectedPath;
            }
        }
    }
}
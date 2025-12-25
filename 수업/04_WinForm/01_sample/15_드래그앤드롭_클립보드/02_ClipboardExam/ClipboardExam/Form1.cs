using System;
using System.IO;
using System.Drawing;
using System.Windows.Forms;

namespace ClipboardExam
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }
   
        private void btn_clipinfo_Click(object sender, EventArgs e)
        {
            if (Clipboard.ContainsImage())
            {
                txt_info.AppendText("1. 이미지 파일 포함되어 있음...\r\n");
                btn_addImage.Enabled = true;
                btn_getImage.Enabled = true;
            }
            else
            {
                txt_info.AppendText("1. 이미지 파일이 포함되지 않음...\r\n");
                btn_addImage.Enabled = true;
            }

            if (Clipboard.ContainsAudio())
            {
                txt_info.AppendText("2. 오디오 파일 포함되어 있음...\r\n");
                btn_addAudio.Enabled = true;
                btn_getAudio.Enabled = true;
            }
            else
            {
                txt_info.AppendText("2. 오디오 파일이 포함되지 않음...\r\n");
                btn_addAudio.Enabled = true;
            }

            if (Clipboard.ContainsText())
            {
                txt_info.AppendText("3. 텍스트 포함되어 있음...\r\n");
                btn_addText.Enabled = true;
                btn_getText.Enabled = true;
            }
            else
            {
                txt_info.AppendText("3. 텍스트 포함되지 않음...\r\n");
                btn_addText.Enabled = true;
            }

        }

        private void btn_addImage_Click(object sender, EventArgs e)
        {
            OpenFileDialog dlg = new OpenFileDialog();
            dlg.Title = "클립보드에 추가할 이미지를 선택하세요~";
            dlg.Filter = "*.bmp | *.bmp | *.gif | *.gif | *.jpg | *.jpg";
            if(dlg.ShowDialog() == DialogResult.OK)
            {
                Image img = Image.FromFile(dlg.FileName);
                Clipboard.SetImage(img);        // 클립보드에 이미지 추가 
                txt_info.AppendText("클립보드에 " + dlg.FileName + " 이미지 추가 \r\n");
            }
        }

        private void btn_getImage_Click(object sender, EventArgs e)
        {
            if (Clipboard.ContainsImage())
            {
                Bitmap bmp = new Bitmap(Clipboard.GetImage());  // 클립보드에서 이미지 추출
                bmp.Save("c:\\클립보드이미지.bmp");
                txt_info.AppendText("클립보드 이미지를 C:\\에 클립보드이미지.bmp로 출력 \r\n");
            }
        }

        private void btn_addAudio_Click(object sender, EventArgs e)
        {
            OpenFileDialog dlg = new OpenFileDialog();
            dlg.Title = "클립보드에 추가할 오디오 파일을 선택하세요~";
            dlg.Filter = "*.wav | *.wav";
            if(dlg.ShowDialog() == DialogResult.OK)
            {
                FileStream fs = File.Open(dlg.FileName, FileMode.Open);
                Clipboard.SetAudio(fs);
                fs.Close();
                txt_info.AppendText("클립보드에 " + dlg.FileName + " 오디오 추가 \r\n");
            }
        }

        private void btn_getAudio_Click(object sender, EventArgs e)
        {
            if (Clipboard.ContainsAudio())
            {
                Stream st = Clipboard.GetAudioStream();
                FileStream fs = File.Create("C:\\클립보드오디오.wav");
                for(int i = 0; i < st.Length; i++)
                    fs.WriteByte((byte)st.ReadByte());
                fs.Close();
            }
        }

        private void btn_addText_Click(object sender, EventArgs e)
        {
            Clipboard.SetText("클립보드에 문자열을 추가합니다.");
            txt_info.AppendText("클립보드에 문자열 추가...\r\n");
        }

        private void btn_getText_Click(object sender, EventArgs e)
        {
            if (Clipboard.ContainsText())
                MessageBox.Show(Clipboard.GetText());
        }
    }
}
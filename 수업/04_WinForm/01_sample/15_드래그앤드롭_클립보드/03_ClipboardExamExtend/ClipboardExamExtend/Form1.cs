using System;
using System.IO;
using System.Drawing;
using System.Windows.Forms;

namespace ClipboardExamExtend
{
    public partial class Form1 : Form
    {
        RichTextBox richtxt;

        public Form1()
        {
            InitializeComponent();

            richtxt = new RichTextBox();
            richtxt.Parent = this.panel1;
            richtxt.SetBounds(0, 0, this.panel1.Width, this.panel1.Height);
            richtxt.Visible = false;

            Timer timer = new Timer();
            timer.Interval = 1000;
            timer.Tick += new EventHandler(timer_Tick);
            timer.Start();
        }

        private void timer_Tick(object sender, EventArgs e)
        {
            this.Text = DateTime.Now.ToLongTimeString();
        }

        private void btn_addTime_Click(object sender, EventArgs e)
        {
            DateTime date = DateTime.Now;

            // 날짜 시간 형식으로 date 저장
            Clipboard.SetData("날짜시간", date);
        }

        private void btn_getTime_Click(object sender, EventArgs e)
        {
            // 날짜 시간 형식의 포멧이 존재하는지 확인 
            if (Clipboard.ContainsData("날짜시간"))
            {
                DateTime date = (DateTime)Clipboard.GetData("날짜시간");
                Graphics grfx = this.panel1.CreateGraphics();
                grfx.DrawString(date.ToLongDateString(), this.Font, Brushes.Yellow, 20, 30);
                grfx.DrawString(date.ToLongTimeString(), this.Font, Brushes.Orange, 20, 50);                
            }
            
        }        

        private void btn_clipboardReset_Click(object sender, EventArgs e)
        {
            Clipboard.Clear();  // 클립보드 초기화
            Graphics grfx = this.panel1.CreateGraphics();
            grfx.FillRectangle(Brushes.Blue, 0, 0, this.panel1.Width, this.panel1.Height);
            this.richtxt.Visible = false;
        }

        private void btn_addRTF_Click(object sender, EventArgs e)
        {
            OpenFileDialog dlg = new OpenFileDialog();
            dlg.Title = "RTF 파일을 선택하세요";
            if (dlg.ShowDialog() == DialogResult.OK)
            {
                richtxt.LoadFile(dlg.FileName);
                Clipboard.SetDataObject(richtxt.Rtf);
            }
        }

        private void btn_getRTF_Click(object sender, EventArgs e)
        {
            IDataObject obj = Clipboard.GetDataObject();
            string rtf = (string)obj.GetData(typeof(string));            
            richtxt.Rtf = rtf;
            richtxt.Visible = true;
        }
    }
}
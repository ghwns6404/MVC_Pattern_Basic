using System;
using System.Drawing;
using System.Windows.Forms;
using System.Drawing.Printing;

namespace PrintPreviewExam
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            PrintDocument pd = new PrintDocument();
            pd.PrintPage += new PrintPageEventHandler(pd_PrintPage);

            PrintPreviewDialog ppd = new PrintPreviewDialog();
            ppd.Document = pd;

            // 인쇄 미리보기 창 출력시 PrintPage 메서드가 실행되게 됨
            ppd.ShowDialog();
        }

        private void pd_PrintPage(object sender, PrintPageEventArgs ev)
        {
            Font font = new Font("굴림", 30, FontStyle.Bold);

            int leftMargin = ev.MarginBounds.Left;   // 인쇄용지의 왼쪽 마진
            int topMargin = ev.MarginBounds.Top;     // 인쇄용지의 위쪽 마진
            int yPos = topMargin + 200;  // 인쇄용지의 위쪽 마진에서 200인 위치에 출력

            string str = "PrintPreview Exam 문장입니다..."; // 용지에 출력할 문장
            ev.Graphics.DrawString(str, font, Brushes.Black,
                leftMargin, yPos, new StringFormat());

            font.Dispose();
        }
    }
}
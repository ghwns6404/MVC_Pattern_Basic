using System;
using System.Drawing;
using System.Windows.Forms;
using System.Drawing.Printing;

namespace PageSetupDlgExam
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            try
            {
                PageSetupDialog psd = new PageSetupDialog();
                PageSettings pageSet = new PageSettings();
                psd.PageSettings = pageSet;
                psd.ShowDialog();

                PrintDocument pd = new PrintDocument();
                pd.PrintPage += new PrintPageEventHandler(pd_PrintPage);

                // pageSettings 값 설정 
                pd.DefaultPageSettings = pageSet;

                PrintPreviewDialog ppd = new PrintPreviewDialog();
                ppd.Document = pd;
                ppd.ShowDialog();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        private void pd_PrintPage(object sender, PrintPageEventArgs ev)
        {
            Font font = new Font("굴림", 50, FontStyle.Bold);
            int leftMargin = ev.MarginBounds.Left;
            string str = "PrintSettings Exam"; // 용지에 출력할 문장
            ev.Graphics.DrawString(str, font, Brushes.Black,
                leftMargin, 200, new StringFormat());
            font.Dispose();
        }

    }
}
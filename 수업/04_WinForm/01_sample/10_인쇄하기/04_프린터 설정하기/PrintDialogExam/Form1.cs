using System;
using System.Drawing;
using System.Windows.Forms;
using System.Drawing.Printing;

namespace PrintDialogExam
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            // 프린터 설정 관련 객체 
            PrinterSettings ps = new PrinterSettings();

            // 프린터 설정 관련 다이얼로그 
            PrintDialog pdlg = new PrintDialog();

            // 설정 내용을 ps에 담음
            pdlg.PrinterSettings = ps;

            // PrintDialog 대화상자 출력 
            pdlg.ShowDialog();

            string info = String.Format(" PrinterName = {0} \r\n PaperSizes = {1}",
                                          ps.PrinterName, ps.Copies);
            MessageBox.Show(info);

            /*
            // 프린트 설정내용을 PrintDocument 객체에 설정
            PrintDocument pd1 = new PrintDocument();
            pd1.PrinterSettings = ps;
            ...
            */
        }
    }
}
using System;
using System.Drawing;
using System.Windows.Forms;
using System.Drawing.Printing;

namespace MyDiary
{
    public partial class Form1 : Form
    {
        private Font m_MainFont = null;
        private Font m_SubFont = null;
        private Font m_SmallFont = null;
        private PageSettings m_PageSetting = null;
        private Bitmap m_backbmp = null;
        private Bitmap[] m_weather = new Bitmap[4];

        public Form1()
        {
            InitializeComponent();
            
            m_MainFont = new Font("궁서체", 15, FontStyle.Bold);
            m_SubFont = new Font("굴림체", 13);
            m_SmallFont = new Font("바탕체", 9);

            m_backbmp = new Bitmap(GetType(), "background.jpg");

            for (int i = 0; i < 4; i++)
            {
                string str = string.Format("weather{0}.gif", i + 1);
                m_weather[i] = new Bitmap(GetType(), str);
            }

            cb_Weather.SelectedIndex = 0;	
        }

        private void btn_PageSetup_Click(object sender, EventArgs e)
        {
            try
            {
                PageSetupDialog psetup = new PageSetupDialog();
                if (this.m_PageSetting == null)
                    this.m_PageSetting = new PageSettings();
                psetup.PageSettings = this.m_PageSetting;
                psetup.ShowDialog();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        private void btn_Preview_Click(object sender, EventArgs e)
        {
            try
            {
                PrintDocument pd = new PrintDocument();
                pd.PrintPage += new PrintPageEventHandler(this.PrintPageEvent);

                if (this.m_PageSetting != null)
                    pd.DefaultPageSettings = this.m_PageSetting;

                PrintPreviewDialog pdlg = new PrintPreviewDialog();
                pdlg.Document = pd;
                pdlg.ShowDialog();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        private void btn_Print_Click(object sender, EventArgs e)
        {
            try
            {
                PrintDocument pd = new PrintDocument();
                pd.PrintPage += new PrintPageEventHandler(this.PrintPageEvent);

                if (this.m_PageSetting != null)
                    pd.DefaultPageSettings = this.m_PageSetting;

                PrintDialog pdlg = new PrintDialog();
                pdlg.Document = pd;
                if (pdlg.ShowDialog() == DialogResult.OK)
                {
                    pd.Print();
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        protected void PrintPageEvent(Object obj, PrintPageEventArgs pea)
        {
            Graphics g = pea.Graphics;
            PaintDocument(g);
            pea.HasMorePages = false;
        }

        private void PaintDocument(Graphics g)
        {
            g.FillRectangle(Brushes.White, 100, 50, 800, 600);
            g.DrawImage(m_backbmp, 100, 50);  // 바탕이미지 채우기

            g.DrawImage(m_weather[cb_Weather.SelectedIndex], 410, 230); // 날씨 아이콘 출력			
            g.DrawString(cb_Weather.SelectedItem.ToString(), this.m_MainFont, Brushes.Brown, 450, 237);
            g.DrawString(this.dtp_Date.Text, this.m_SmallFont, Brushes.Brown, 370, 280);

            StringFormat sf = new StringFormat();
            sf.Alignment = StringAlignment.Center;
            sf.LineAlignment = StringAlignment.Center;
            Rectangle rect = new Rectangle(100, 50, 400, this.m_MainFont.Height * 3);
            g.DrawString(this.txt_Title.Text, this.m_MainFont, Brushes.Black, rect, sf);

            rect = new Rectangle(110, 130, 400, this.m_SubFont.Height * 10);
            g.DrawString(this.txt_Content.Text, this.m_SubFont, Brushes.Black, rect);
        }
    }
}
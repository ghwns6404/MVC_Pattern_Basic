using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Security.Cryptography;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace _0422_Form
{
    public partial class Form3_Color : Form
    {
        public Form3_Color()
        {
            InitializeComponent();
        }
        protected override void OnPaint(PaintEventArgs pea)
        {
            Graphics grfx = pea.Graphics;
            SolidBrush br = new SolidBrush(this.ForeColor);
            Font font = new Font("돋음", 20);
            grfx.DrawString("글자색 변경", font, br, 10, 80);
        }

        private void button1_Click(object sender, EventArgs e)
        {
            ColorDialog colorDlg    = new ColorDialog();
            colorDlg.AllowFullOpen  = false;
            colorDlg.ShowHelp       = true;

            if ((Button)sender == button1)  // 배경색 변경
            {
                colorDlg.Color = this.BackColor;
                if (colorDlg.ShowDialog() == DialogResult.OK)
                    this.BackColor = colorDlg.Color;
            }
            else  // 전경색 변경
            {
                colorDlg.Color = this.ForeColor;
                if (colorDlg.ShowDialog() == DialogResult.OK)
                    this.ForeColor = colorDlg.Color;
            }
        }

        private void button3_Click(object sender, EventArgs e)
        {
            ColorDialog colorDlg = new ColorDialog();
            colorDlg.ShowHelp = true;

            if (colorDlg.ShowDialog() == DialogResult.OK)
            {
                using (Graphics g = pictureBox1.CreateGraphics())
                {
                    pictureBox1.BackColor = colorDlg.Color;

                    textBox1.Text = colorDlg.Color.Name;
                    textBox2.Text = string.Format("0x{0:X}", ((uint)colorDlg.Color.ToArgb()));
                    textBox3.Text = colorDlg.Color.A.ToString();
                    textBox4.Text = colorDlg.Color.R.ToString();
                    textBox5.Text = colorDlg.Color.G.ToString(); ;
                    textBox6.Text = colorDlg.Color.B.ToString(); ;
                }
            }  
        }
    }
}

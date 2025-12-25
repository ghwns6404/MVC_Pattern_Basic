using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace FlowLayoutExam
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            // 가로 늘리기
            this.Size = new Size(600, 200);
        }

        private void button2_Click(object sender, EventArgs e)
        {
            // 세로 늘리기
            this.Size = new Size(300, 300);
        }
    }
}
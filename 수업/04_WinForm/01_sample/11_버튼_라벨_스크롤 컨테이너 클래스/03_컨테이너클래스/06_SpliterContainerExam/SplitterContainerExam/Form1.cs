using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace SplitterContainerExam
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void btn_splitop_Click(object sender, EventArgs e)
        {
            this.splitContainer1.SplitterWidth = Int32.Parse(this.textBox1.Text);
            if(this.splitContainer1.Orientation == Orientation.Horizontal)
            {
                this.splitContainer1.Orientation = Orientation.Vertical;
                this.btn_splitop.Text = "수평 스플리트";
            }
            else if (this.splitContainer1.Orientation == Orientation.Vertical)
            {
                this.splitContainer1.Orientation = Orientation.Horizontal;
                this.btn_splitop.Text = "수직 스플리트";
            }
            
        }

        private void btn_panel2_Click(object sender, EventArgs e)
        {
            if (btn_panel2.Text == "Panel2 숨기기")
            {
                this.splitContainer1.Panel2Collapsed = true;
                btn_panel2.Text = "Panel2 보이기";
            }
            else
            {
                this.splitContainer1.Panel2Collapsed = false;
                btn_panel2.Text = "Panel2 숨기기";
            }
        }

        private void btn_panel1_Click(object sender, EventArgs e)
        {
            if (btn_panel1.Text == "Panel1 숨기기")
            {
                this.splitContainer1.Panel1Collapsed = true;
                btn_panel1.Text = "Panel1 보이기";
            }
            else
            {
                this.splitContainer1.Panel1Collapsed = false;
                btn_panel1.Text = "Panel1 숨기기";
            }
       
        }      
    }
}
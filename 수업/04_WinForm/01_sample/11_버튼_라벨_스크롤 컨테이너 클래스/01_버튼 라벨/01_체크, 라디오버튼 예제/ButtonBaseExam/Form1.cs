using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace ButtonBaseExam
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            string message = "취미는";
            if(checkBox1.Checked)
            {
                message += "\n -" + checkBox1.Text;
            }
            if (checkBox2.Checked)
            {
                message += "\n -" + checkBox2.Text;
            }
            if (checkBox3.Checked)
            {
                message += "\n -" + checkBox3.Text;
            }
            message += "\n성별은 ";
            if(radioButton1.Checked)
            {
                message += radioButton1.Text;
            }
            else if(radioButton2.Checked)
            {
                message += radioButton2.Text;
            }

            MessageBox.Show(message);
        }       
    }
}
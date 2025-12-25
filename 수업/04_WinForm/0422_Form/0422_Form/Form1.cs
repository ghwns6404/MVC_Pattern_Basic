using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace _0422_Form
{
    public partial class Form1 : Form
    {
        private TextBox mytext = new TextBox();

        public Form1()
        {
            InitializeComponent();

            mytext.Location = new Point(0, 0);
            mytext.Name = "mytext";
            mytext.Size = new Size(100, 20);
            this.Controls.Add(mytext);
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            MessageBox.Show("Load");
        }

        private void button1_Click(object sender, EventArgs e)
        {
            string str = textBox1.Text;
            label1.Text = str; 
        }
    }
}

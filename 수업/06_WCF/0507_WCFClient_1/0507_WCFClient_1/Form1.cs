using _0507_WCFClient_1.ServiceReference1;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;


namespace _0507_WCFClient_1
{
    public partial class Form1 : Form
    {
        private HelloWorldClient c = new HelloWorldClient();
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            textBox1.Text = c.SayHello();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            c.setNumber(10, 20);
            textBox2.Text = c.GetSumNumber().ToString();
        }
    }
}

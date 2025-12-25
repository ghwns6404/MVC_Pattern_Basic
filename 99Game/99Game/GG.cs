using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace _99Game
{
    public partial class GG : Form
    {
        Form1 f1;
        public GG(int score, Form1 form1)
        {
            InitializeComponent();
            Ending(score);
            f1 = form1;
        }
        public void Ending(int score)
        {
            finalscoreBox.Text = score.ToString();
        }

        private void button1_Click(object sender, EventArgs e)//재시작 버튼
        {
            DialogResult = DialogResult.OK;
            f1.Start();
        }
    
    }
}

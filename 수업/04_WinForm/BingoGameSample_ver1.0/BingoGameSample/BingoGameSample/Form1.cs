using System;
using System.Windows.Forms;

namespace BingoGameSample
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();     
        }

        #region 게임준비 
        private void button1_Click(object sender, EventArgs e)
        {
            //bingoSingleBoard1.IsShow = false;
            bingoSingleBoard1.SetNumberRandom();
        }
        #endregion

        private void bingoSingleBoard1_NumberClick(object Sender, NumberEventArgs e)
        {   //숫자를 누르면 폼텍스트칸에 누른 숫자와 좌표 출력
            this.Text = string.Format($" 숫자 :{e.Number}\t\t좌표: [{e.X}, {e.Y}]");
        }
    }
}

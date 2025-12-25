using System;
using System.Windows.Forms;
using static 관리프로그램Vol._2.Form1;


namespace 관리프로그램Vol._2
{
    public partial class LoginForm : Form
    {
        MyDB db = new MyDB();
        public LoginForm()
        {
            InitializeComponent();
            db.Connect();
        }

        private void button5_Click(object sender, EventArgs e)
        {
            string id = textBox12.Text;
            string pw = textBox11.Text;
            bool LoginCheck = false;

            foreach (Data data in db.SelectDB())
            {
                if (data.Id == id && data.Pw == pw)
                {
                    LoginCheck = true;
                    break;
                }
                else
                    LoginCheck = false;
            }
            if (LoginCheck == true)
                MessageBox.Show("로그인 성공");
            else
                MessageBox.Show("로그인 실패", "로그인 실패", MessageBoxButtons.OK, MessageBoxIcon.Error);
        }
        
    }
}

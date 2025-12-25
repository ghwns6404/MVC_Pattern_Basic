using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace _0424_ChatClient
{
    public partial class LogInForm : Form
    {
        private MemberControl memControl = MemberControl.Singleton;

        #region 컨트롤 바인딩 LogId, LogPw
        public string LogId
        {
            get { return textBox1.Text;  }
            set { textBox1.Text = value; }
        }
        public string LogPw
        {
            get { return textBox2.Text; }
            set { textBox2.Text = value; }
        }
        #endregion 

        public LogInForm()
        {
            InitializeComponent();
            memControl._LogInForm = this;   //*******************
        }

        #region 로그인 버튼
        private void button1_Click(object sender, EventArgs e)
        {
            memControl.LogIn(LogId, LogPw);
        }
        public void LogIn_Result(bool isflag, string id, string name)
        {
            if(isflag == true)
            {
                this.BeginInvoke(new MethodInvoker(delegate ()
                {
                    this.Hide();
                    ChatForm chatForm = new ChatForm();
                    chatForm.ChatId = LogId;     //***************
                    chatForm.ShowDialog();
                    this.Show();
                }));

            }
            else
            {
                MessageBox.Show("로그인 실패");
            }
        }

        #endregion

        #region 회원가입 버튼
        private void button2_Click(object sender, EventArgs e)
        {
            this.BeginInvoke(new MethodInvoker(delegate () {
                this.Hide();
                AddMemberForm addMemberForm = new AddMemberForm();
                if (addMemberForm.ShowDialog() == DialogResult.OK)
                    MessageBox.Show("회원가입 성공");
                this.Show();
            }));
        }
        #endregion

        #region Form_Load, Form_Closed : ini 파일 저장 및 불러오기
        private void LogInForm_Load(object sender, EventArgs e)
        {
            try
            {
                string mip, cip, id, pw;
                int mport, cport;
                MyInfoFile.ReadMesssage(out mip, out mport, out cip, out cport, out id, out pw);
                LogId = id;
                LogPw = pw;

                //네트웤 서버 연결
                memControl.Init(mip, mport);
            }
            catch (Exception ex) 
            { 
                MessageBox.Show(ex.Message);
                this.Close();
            }
        }

        private void LogInForm_FormClosed(object sender, FormClosedEventArgs e)
        {
            MyInfoFile.WriteMesssage(LogId, LogPw);

            //네트웤 서버 연결 종료
            memControl.Exit();
        }
        #endregion
    }
}

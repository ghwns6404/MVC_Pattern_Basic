using System;
using System.ServiceModel;
using System.Windows.Forms;
using _0508_ChatClient.ServiceReference1;

namespace _0508_ChatClient
{
    public partial class Form1 : Form, IChatCallback
    {
        public string NickName
        {            
            get { return textBox1.Text;  }
            set { textBox1.Text = value; }
        }

        public string ShortMsg
        {
            get { return textBox3.Text; }
            set { textBox3.Text = value; }
        }

        private ChatClient _chatClient = null;
        public Form1()
        {
            InitializeComponent();

            NickName = "홍길동";
            _chatClient = new ChatClient(new InstanceContext(this));
        }

        #region 메시지 수신
        public void Receive(string nickname, string msg, DateTime date)
        {            
            string temp = string.Format($"[{nickname}] {msg} ({date.ToShortTimeString()})");

            if (textBox2.InvokeRequired)
            {
                textBox2.Invoke(new MethodInvoker(delegate ()
                {
                    textBox2.AppendText(temp + "\r\n");
                }));
            }
            else
            {
                textBox2.AppendText(temp + "\r\n");
            }
        }

        public void UserEnter(string nickname, DateTime date)
        {
            string temp = string.Format($"{nickname}님이 로그인하셨습니다. ({date.ToShortTimeString()})");
            if (textBox2.InvokeRequired)
            {
                textBox2.Invoke(new MethodInvoker(delegate ()
                {
                    textBox2.AppendText(temp + "\r\n");
                }));
            }
            else
            {
                textBox2.AppendText(temp + "\r\n");
            }
        }

        public void UserLeave(string nickname, DateTime date)
        {
            string temp = string.Format($"{NickName}님이 로그아웃하셨습니다. ({DateTime.Now})");
            if (textBox2.InvokeRequired)
            {
                textBox2.Invoke(new MethodInvoker(delegate ()
                {
                    textBox2.AppendText(temp + "\r\n");
                }));
            }
            else
            {
                textBox2.AppendText(temp + "\r\n");
            }
        }
        #endregion

        #region 메시지 전송
        //Join
        private void button2_Click(object sender, EventArgs e)
        {
            if (_chatClient.Join(NickName, DateTime.Now) == false)
            {
                MessageBox.Show("이미 존재하는 nickname입니다.");
            }

        }

        //Leave
        private void button3_Click(object sender, EventArgs e)
        {

            _chatClient.Leave(NickName, DateTime.Now);

            string temp = string.Format($"{NickName}님이 로그아웃하셨습니다. ({DateTime.Now})");
            textBox2.AppendText(temp + "\r\n");
        }

        //Send
        private void button1_Click(object sender, EventArgs e)
        {
            try
            {
                _chatClient.Say(NickName, ShortMsg, DateTime.Now);
            }
            catch (Exception)
            {
                MessageBox.Show("Join을 먼저");
            }
        }
        #endregion 
    }
}

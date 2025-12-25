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

        public void Receive(string nickname, string msg, DateTime date)
        {            
            string temp = string.Format($"[{nickname}] {msg} ({date.ToShortTimeString()})");
            textBox2.AppendText(temp + "\r\n");
        }

        public void UserEnter(string nickname, DateTime date)
        {
            string temp = string.Format($"{nickname}님이 로그인하셨습니다. ({date.ToShortTimeString()})");
            textBox2.AppendText(temp + "\r\n");
        }

        //Join
        private void button2_Click(object sender, EventArgs e)
        {
            _chatClient.Join(NickName, DateTime.Now);
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
    }
}

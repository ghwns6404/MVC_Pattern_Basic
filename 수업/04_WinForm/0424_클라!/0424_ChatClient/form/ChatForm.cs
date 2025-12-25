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
    public partial class ChatForm : Form
    {
        private MemberControl memControl    = MemberControl.Singleton;
        private ChatControl chatControl     = ChatControl.Singleton;

        public string ChatId { get; set; }

        #region 컨트롤 바인딩 : SendText
        public string SendText
        {
            get { return textBox2.Text;  }
            set { textBox2.Text = value; }
        }
        #endregion

        public ChatForm()
        {
            InitializeComponent();
            memControl._ChatForm = this;
        }

        #region 로그아웃 버튼, FormClose : 로그아웃 요청
        private void button1_Click(object sender, EventArgs e) 
        {
            memControl.LogOut(ChatId);            
        }
        public void LogOut_Result(bool isflag, string id, string name) //이새끼
        {
            this.BeginInvoke(new MethodInvoker(delegate ()
            {
                if (isflag)
            {
                this.Close();
            }
            }));
        }
        private void ChatForm_FormClosed(object sender, FormClosedEventArgs e)
        {
            memControl.LogOut(ChatId);
        }
        #endregion

        #region 데이터 전송, 엔터키 처리
        private void button2_Click(object sender, EventArgs e)
        {
            chatControl.SendMessage(ChatId, SendText, DateTime.Now);
            AppendTextView(ChatId, SendText, DateTime.Now);
            SendText = "";
        }
        private void textBox2_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                button2_Click(sender, e);
            }
        }
        #endregion

        #region 텍스트 다중 라인 출력
        public void AppendTextView(string id, string text, DateTime date)
        {
            string msg = string.Format($"[{id}] {text} ({date.ToShortTimeString()})");
            textBox1.AppendText(msg + "\r\n");
        }

        #endregion 
    }
}

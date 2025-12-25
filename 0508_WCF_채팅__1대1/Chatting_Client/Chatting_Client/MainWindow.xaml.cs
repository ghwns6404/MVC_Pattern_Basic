using Chatting_Client.ServiceReference1;
using System;
using System.ServiceModel;
using System.Windows;


namespace Chatting_Client
{
    /// <summary>
    /// MainWindow.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class MainWindow : Window, IChatCallback
    {
        string nickname;
        private IChat chat;

        public MainWindow()
        {
            InitializeComponent();
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            //2 ===================================================
            InstanceContext site = new InstanceContext(this);
            chat = new Chatting_Client.ServiceReference1.ChatClient(site);
        }

        #region IChatCallback 인터페이스 함수 생성
        public void Receive(string nickname, string msg, DateTime date)
        {
            string msgtemp = string.Format("[{0}] :  {1}\t\t{2}",nickname, msg, date);
            chatlist.Items.Add(msgtemp);
        }
        public void UserEnter(string nickname, DateTime date)
        {
            string msgtemp = string.Format("{0}님이 로그인하셨습니다. {1}", nickname, date);
            chatlist.Items.Add(msgtemp);
        }

       
        #endregion


        #region 로그인/로그아웃 핸들러
        private void btnJoin_Click(object sender, RoutedEventArgs e)
        {
            if ((string)btnJoin.Content == "로그인") this.Connect();
            else this.DisConnect();
        }

        private void Connect()
        {
            try
            {
                nickname = seatbox.Text;
                DateTime date = DateTime.Now;

                //서버 접속
                if (chat.Join(nickname, date))
                {
                    MessageBox.Show("접속 성공");
                    UserEnter(nickname, date);
                }
                else
                    MessageBox.Show("접속 실패");



                btnJoin.Content = "로그아웃";
                string login = string.Format("{0}님이 로그인하셨습니다.", seatbox.Text);
            }
            catch (Exception ex)
            {
                MessageBox.Show("접속 오류 :{0}", ex.Message);
            }
        }

        private void DisConnect()
        {
            try
            {
                DateTime date = DateTime.Now;
                chat.Leave(nickname, date);

                btnJoin.Content = "로그인";

                string logout = string.Format("{0}님이 로그아웃하셨습니다.", seatbox.Text);
                chatlist.Items.Add(logout);
            }
            catch (Exception ex)
            {
                MessageBox.Show("나가기 오류 :{0}", ex.Message);
            }
        }
        #endregion

        // 메시지 전송
        private void btnSend_Click(object sender, RoutedEventArgs e)
        {
            if (btnJoin.Content.Equals("로그아웃"))
            {
                DateTime date = DateTime.Now;
                string msg = msgbox.Text;

                string temp = string.Format("[{0}]", msg);

                chat.Say(nickname, msg, date);

                Receive(nickname, msg, date);

                msgbox.Clear();
            }
            else
                MessageBox.Show("로그인을 하시오");
        }  
    }
}


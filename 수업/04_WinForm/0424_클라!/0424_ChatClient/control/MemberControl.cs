using System;
using System.Drawing;
using System.Net.Sockets;
using WoosongBit41.ClientNet;

namespace _0424_ChatClient
{
    internal class MemberControl
    {
        private MyClient        _client = new MyClient();
        public LogInForm        _LogInForm { get; set; }        = null;
        public AddMemberForm    _AddMemberForm { get; set; }    = null;
        public ChatForm         _ChatForm { get; set; }         = null;

        #region 싱글톤 패턴
        public static MemberControl Singleton { get; } = null;
        static MemberControl() { Singleton = new MemberControl(); }
        private MemberControl()
        {
        }
        #endregion

        #region 기능(로그인)
        public bool LogIn(string id, string pw)
        {
            string packet = Packet.MemberLogIn(id, pw); 
            _client.SendData(packet);
            return true;
        }
        private void LogIn_Ack(string msg)   //isflag, id, name
        {
            string[] sp = msg.Split('#');
            bool isflag = bool.Parse(sp[0]);
            string id   = sp[1];
            string name = sp[2];
            _LogInForm.LogIn_Result(isflag, id, name); 
        }
        #endregion

        #region 기능(로그아웃)
        public bool LogOut(string id)
        {
            string packet = Packet.MemberLogOut(id);
            _client.SendData(packet);
            return true;
        }
        private void LogOut_Ack(string msg) //isflag, id, name
        {
            string[] sp = msg.Split('#');
            bool isflag = bool.Parse(sp[0]);
            string id = sp[1];
            string name = sp[2];
            _ChatForm.LogOut_Result(isflag, id, name);
        }
        #endregion

        #region 기능(회원가입)
        public bool AddMember(string id, string pw, string name, Color color, DateTime date)
        {
            string packet = Packet.MemberAddMember(id, pw, name, color, date);
            _client.SendData(packet);
            return true;
        }
        private void AddMember_Ack(string msg) //isflag, id, name
        {
            string[] sp     = msg.Split('#');
            bool isflag     = bool.Parse(sp[0]);
            string id       = sp[1];
            string name     = sp[2];
            _AddMemberForm.AddMember_Result(isflag, id, name);
        }
        #endregion

        #region CallBack Message
        public void LogMessage(Socket sock, string message)
        {
            Console.WriteLine($"[log] {message}");
        }
        public void PacketMessage(Socket sock, string message)
        {
            //1. 패킷 수신
            Console.WriteLine($"[packet] {message}");

            //2. 패킷 파싱(분석) 및 분할 처리
            string[] sp = message.Split('@');
            switch (int.Parse(sp[0]))
            {
                case Packet.PACKET_MEMBER_LOGIN_ACK:        LogIn_Ack(sp[1]);     break;
                case Packet.PACKET_MEMBER_LOGOUT_ACK:       LogOut_Ack(sp[1]);    break;
                case Packet.PACKET_MEMBER_ADDMEMBER_ACK:    AddMember_Ack(sp[1]); break;
             }
        }
        #endregion

        #region 시작과 종료 메서드
        public void Init(string ip, int port)
        {
            if (_client.Start(ip, port, LogMessage, PacketMessage) == false)
                throw new Exception("회원 관리서버 연결 오류");
        }
        public void Exit()
        {
            _client.Close();
        }
        #endregion
    }
}

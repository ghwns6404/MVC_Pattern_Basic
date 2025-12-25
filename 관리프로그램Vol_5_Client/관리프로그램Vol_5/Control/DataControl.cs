//DataControl.cs
using _관리프로그램Vol_5;
using System;
using System.Net.Sockets;

namespace 관리프로그램Vol_5
{
    internal class DataControl
    {
        private MyClient _client = new MyClient();
        public Form1 _Form1 { get; set; } = null;
        public LoginForm _LoginForm { get; set; } = null;

        #region 싱글톤 패턴
        public static DataControl Singleton { get; } = null;
        static DataControl() { Singleton = new DataControl(); }
        private DataControl()
        {
        }
        #endregion

        #region 1. 회원 가입
        public bool Register(string id, string pw, string name, string age, string phone)
        {
            string packet = Packet.MakeRegister(id, pw, name, age, phone);
            _client.SendData(packet); 
            return true;
        }
        public void Register_Ack(string msg)
        {
            string[] sp = msg.Split('#');
            bool isflag = bool.Parse(sp[0]);
            string id = sp[1];
            string pw = sp[2];
            string name = sp[3];
            string age = sp[4];
            string phone = sp[5];

            _Form1.SetRegister(isflag, id, pw, name, age, phone);
        }
        #endregion

        #region 2. 로그인
        public bool Login(string id, string pw)
        {
            string packet = Packet.MakeLogin(id, pw);
            _client.SendData(packet);
            return true;
        }
        public void Login_Ack(string msg)
        {
            string[] sp = msg.Split('#');
            bool isflag = bool.Parse(sp[0]);
            string id = sp[1];
            string pw = sp[2];

            _LoginForm.SetLogin(isflag, id, pw);
        }
        #endregion

        #region 콜백메세지
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
                case Packet.PACKET_DATA_REGISTER_ACK: Register_Ack(sp[1]); break;
                case Packet.PACKET_DATA_LOGIN_ACK: Login_Ack(sp[1]);       break;
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

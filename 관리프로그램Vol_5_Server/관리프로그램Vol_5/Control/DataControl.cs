//DataControl.cs
using _관리프로그램Vol_5;
using System;
using System.Net.Sockets;
using System.Windows.Forms;

namespace 관리프로그램Vol_5
{
    internal class DataControl
    {
        MyDB db = new MyDB();

        //network
        private const int SERVER_PORT = 9000;
        private MyServer _server = new MyServer();

        public Form1 _Form1 { get; set; } = null;
        public LoginForm _LoginForm { get; set; } = null;
        public SearchForm _SearchForm { get; set; } = null;
        public LogsForm _LogsForm { get; set; } = null;

        #region 싱글톤 패턴
        public static DataControl Singleton { get; } = null;
        static DataControl() { Singleton = new DataControl(); }
        private DataControl()
        {
        }
        #endregion

        #region 1. 회원 가입
        public bool Register(Socket socket, string message)
        {
            try
            {
                //1. 데이터 획득 --------------------------------------------
                string[] sp = message.Split('#');
                string id = sp[0];
                string pw = sp[1];
                string name = sp[2];
                string age = sp[3];
                string phone = sp[4];


                //2. 데이터 처리 ---------------------------------------------
                db.InsertDB(id, pw, name, age, phone);
                //3. 데이터 전송 ----------------------------------------------
                string packet = Packet.MakeRegister(true, id, pw, name, age, phone);
                _server.SendData(socket, packet);
                return true;
            }
            catch (Exception ex)
            {
                
                string packet = Packet.MakeRegister(false, ex.Message, "--", "--", "--", "D.C의 Register");
                _server.SendData(socket, packet);
                return false;
            }
        }
        
        #endregion

        #region 2. 로그인
        public bool Login(Socket socket, string message)
        {
            try
            {
                //1. 데이터 획득 --------------------------------------------
                string[] sp = message.Split('#');
                string id = sp[0];
                string pw = sp[1];

                //2. 데이터 전송----------------------------------------------
                if(db.selectDB_Compare(id, pw) == true)
                {
                    string packet = Packet.MakeLogin(true, id, pw);
                    _server.SendData(socket, packet);
                    return true;
                }
                else
                {
                    MessageBox.Show("compare에서 붐");
                    return false;
                }
            }
            catch (Exception ex)
            {
                string packet = Packet.MakeLogin(false, ex.Message, "----D.C의 Login");
                _server.SendData(socket, packet);
                return false;
            }
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
                case Packet.PACKET_DATA_REGISTER: Register(sock, sp[1]); break;
                case Packet.PACKET_DATA_LOGIN: Login(sock, sp[1]);       break;
            }
        }
        #endregion

        #region 2. 시작과 종료 메서드
        public void Init()
        {
            db.Connect();
            if (_server.Start(SERVER_PORT, LogMessage, PacketMessage) == false)
                return;
            Console.WriteLine("서버 실행........");
        }
        public void Exit()
        {
            db.Close();
            _server.Close();
            Console.WriteLine("서버 종료........");
        }
        #endregion
    }
}

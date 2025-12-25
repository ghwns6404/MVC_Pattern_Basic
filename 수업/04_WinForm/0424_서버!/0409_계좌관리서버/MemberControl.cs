//AccountControl.cs
using System;
using System.Collections.Generic;
using WoosongBit41.ServerNet;
using WoosongBit41.Data;
using System.Net.Sockets;
using System.Security.Policy;
using System.Drawing;
using System.Data;
using System.Windows;

namespace _0409_계좌관리서버
{
    internal class MemberControl
    {
        MyDB db = new MyDB();
        
        //network
        private const int SERVER_PORT   = 9000;
        private MyServer _server        = new MyServer();
        
        #region 0. 싱글톤 패턴
        public static MemberControl Singleton { get; } = null;
        static MemberControl() { Singleton = new MemberControl(); }
        private MemberControl()
        {
        }
        #endregion

        #region 1. 기능 메서드
        public void MemberLogIn(Socket socket, string message)// 로그인
        {
            try
            {
                //1. 데이터 획득 --------------------------------------------
                string[] sp = message.Split('#');
                string id   = sp[0];
                string pw   = sp[1];
                DateTime date = DateTime.Parse(sp[2]);

                //2. 데이터 전송----------------------------------------------
                string name = db.Select_LogIn(id, pw);  
                db.Update_LogIn(id, pw, true);

                //3. 데이터 전송 ----------------------------------------------
                string packet = Packet.MemberLogIn(true, id, name);
                _server.SendData(socket, packet);
            }
            catch (Exception ex)
            {
                string packet = Packet.MemberLogIn(false, ex.Message, "-");
                _server.SendData(socket, packet);
            }
        }
        public void MemberLogOut(Socket socket, string message) //로그아웃
        {
            try
            {
                //1. 데이터 획득 --------------------------------------------
                string[] sp     = message.Split('#');
                string id       = sp[0];
                DateTime date   = DateTime.Parse(sp[1]);

                //2. 데이터 처리 ---------------------------------------------
                db.Select_LogOut(id);
                db.Update_LogInAndOut(id, false);

                string name = "db에서 획득";
                //3. 데이터 전송 ----------------------------------------------
                string packet = Packet.MemberLogOut(true, id, name);
                _server.SendData(socket, packet);
            }
            catch (Exception ex)
            {
                string packet = Packet.MemberLogOut(false, ex.Message, "-");
                _server.SendData(socket, packet);
            }
        }
        public void MemberAddMember(Socket socket, string message)//가입
        {
            try
            {
                //1. 데이터 획득 --------------------------------------------
                string[] sp     = message.Split('#');
                string id       = sp[0];
                string pw       = sp[1];
                string name     = sp[2];
                Color color     = Color.FromName(sp[3]);    //*** 이름이 들어오는지?
                DateTime date   = DateTime.Parse(sp[4]);

                //2. 데이터 처리 ---------------------------------------------
                db.Insert_AddMemeber(id, pw, name, color, date);
                //3. 데이터 전송 ----------------------------------------------
                string packet = Packet.MemberAddMember(true, id, name);
                _server.SendData(socket, packet);
            }
            catch (Exception ex)
            {
                string packet = Packet.MemberAddMember(false, ex.Message, "-");
                _server.SendData(socket, packet);
                
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

        #region 네트워크 CallBack 메서드 -> 기능 메서드
        public void LogMessage(Socket sock, string message)
        {
            Console.WriteLine($"[log] {message}");
        }
        public void PacketMessage(Socket sock, string message)
        {
            //1. 패킷 수신
            Console.WriteLine($"[packet] {message}");

            //2. 패킷 파싱(분석)
            string[] sp = message.Split('@');

            //3. 분석 분할 처리
            switch (int.Parse(sp[0]))
            {
                case Packet.PACKET_MEMBER_LOGIN:     MemberLogIn(sock, sp[1]);      break;
                case Packet.PACKET_MEMBER_LOGOUT:    MemberLogOut(sock, sp[1]);     break;
                case Packet.PACKET_MEMBER_ADDMEMBER: MemberAddMember(sock, sp[1]);  break;
             }
        }
        #endregion 
    }
}
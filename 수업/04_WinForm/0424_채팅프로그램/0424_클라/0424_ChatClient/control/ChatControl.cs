using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;
using WoosongBit41.ClientNet;

namespace _0424_ChatClient
{
    internal class ChatControl
    {
        private MyClient _client = new MyClient();

        #region 싱글톤 패턴
        public static ChatControl Singleton { get; } = null;
        static ChatControl() { Singleton = new ChatControl(); }
        private ChatControl()
        {
        }
        #endregion

        public void SendMessage(string id, string msg, DateTime date)
        {

        }

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
            /*
            switch (int.Parse(sp[0]))
            {
                case Packet.PACKET_INSERT_ACCOUNT_ACK: AccountInsert_Ack(sp[1]); break;
                case Packet.PACKET_SELECT_ACCOUNT_ACK: AccountSelect_Ack(sp[1]); break;
                case Packet.PACKET_INPUT_ACCOUNT_ACK: AccountInput_Ack(sp[1]); break;
                case Packet.PACKET_OUTPUT_ACCOUNT_ACK: AccountOutput_Ack(sp[1]); break;
                case Packet.PACKET_DELETE_ACCOUNT_ACK: AccountDelete_Ack(sp[1]); break;
                case Packet.PACKET_INPUTOUTPUT_ACCOUNT_ACK: AccountInputOutput_Ack(sp[1]); break;
                case Packet.PACKET_PRINTALL_ACCOUNT_ACK: AccountPrintAll_Ack(sp[1]); break;
            }
            */
        }
        #endregion

        #region 시작과 종료 메서드
        public bool Init(string ip, int port)
        {
            return _client.Start(ip, port, LogMessage, PacketMessage);
        }
        public void Exit()
        {
            _client.Close();
        }
        #endregion
    }
}

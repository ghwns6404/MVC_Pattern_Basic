//MyClient.cs
using System;
using System.Net.Sockets;
using System.Net;
using System.Text;
using System.Threading;

namespace WoosongBit41.ClientNet
{
    internal delegate void LogMessage(Socket sock, string message);
    internal delegate void PacketMessage(Socket sock, string message);

    internal class MyClient : IDisposable
    {
        private Socket socket = null;
        public LogMessage LogCallBack { get; set; } = null;
        public PacketMessage PacketCallBack { get; set; } = null;

        #region 1. Start 호출 시 소켓 생성 및 서버 연결, 수신thread 실행
        public bool Start(string ip, int port, LogMessage log, PacketMessage pack)
        {
            try
            {
                LogCallBack = log;
                PacketCallBack = pack;

                socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
                socket.Connect(new IPEndPoint(IPAddress.Parse(ip), port));
                Thread tr = new Thread(WorkThread);
                tr.IsBackground = true;
                tr.Start();
                return true;
            }
            catch (Exception ex)
            {
                LogCallBack(null, string.Format("[Start 오류] " + ex.Message));
                return false;
            }
        }
        private void WorkThread()
        {
            while (true)
            {
                try
                {
                    //1. 수신
                    byte[] data = null;
                    RecvData(socket, ref data);

                    //2. 수신 정보 출력
                    string msg = Encoding.Default.GetString(data);
                    PacketCallBack(socket, msg);
                }
                catch (Exception ex)
                {
                    LogCallBack(null, string.Format("[recv 오류] " + ex.Message));
                    break;
                }
            }
        }

        #endregion 

        #region 2. Dispose 나 Close 호출 시 소켓 및 스레드 종료
        ~MyClient()
        {
            Dispose();
        }
        public void Dispose()
        {
            Close();
            GC.SuppressFinalize(this);
        }

        public void Close()
        {
            socket.Close();
            LogCallBack(socket, string.Format("[클라이언트 종료] "));
        }
        #endregion

        #region 3. 송수신
        public void SendData(string msg)
        {
            byte[] data = Encoding.Default.GetBytes(msg);
            SendData(socket, data);
        }
        private void SendData(Socket sock, byte[] data)
        {
            try
            {
                //1. [헤더 수신] 가변 데이터 크기
                byte[] head = new byte[4];
                int size = data.Length;
                head = BitConverter.GetBytes(size);
                int send_data = sock.Send(head);

                //2. [데이터 송신] 가변 데이터를 size의 크기만큼 송신
                int total = 0;
                int left_data = size;
                while (total < size)
                {
                    send_data = sock.Send(data, total, left_data, SocketFlags.None);
                    total += send_data;
                    left_data -= send_data;
                }
            }
            catch (Exception ex)
            {
                throw ex;
            }
        }
        private void RecvData(Socket socket, ref byte[] data)
        {
            try
            {
                //1. [헤더 수신] 가변 데이터 크기
                byte[] head = new byte[4];
                int recv_data = socket.Receive(head, 0, head.Length, SocketFlags.None);
                int size = BitConverter.ToInt32(head, 0);   //받을 크기                 

                //2. [데이터 수신] 가변 데이터를 size의 크기만큼 수신
                int total = 0;        //받은 크기
                int left_data = size;     //남은 크기
                data = new byte[size];
                while (total < size)
                {
                    recv_data = socket.Receive(data, total, left_data, 0);
                    if (recv_data == 0) throw new Exception("상대방 소켓 종료");
                    total += recv_data;
                    left_data -= recv_data;
                }
            }
            catch (Exception ex)
            {
                throw ex;
            }
        }
        #endregion
    }
}
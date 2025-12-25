// Packet.cs

namespace 관리프로그램Vol_5
{
    internal class Packet
    {
        public const int PACKET_DATA_REGISTER = 1;
        public const int PACKET_DATA_REGISTER_ACK = 2;

        public const int PACKET_DATA_LOGIN = 3;
        public const int PACKET_DATA_LOGIN_ACK = 4; 

        #region 서버 --> 클라

        public static string MakeRegister(bool ischeck, string id, string pw, string name, string age, string phone)
        {
            string packet = PACKET_DATA_REGISTER_ACK + "@";

            packet += ischeck + "#";
            packet += id + "#";
            packet += pw + "#";
            packet += name + "#";
            packet += age + "#";
            packet += phone;
            return packet;
        }
        public static string MakeLogin(bool ischeck, string id, string pw)
        {
            string packet = PACKET_DATA_LOGIN_ACK+ "@";
            packet += ischeck + "#";
            packet += id + "#";
            packet += pw;
            return packet;
        }   
        #endregion
    }
}

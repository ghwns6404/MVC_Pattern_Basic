//Packet.cs
namespace 관리프로그램Vol_5
{
    internal class Packet
    {
        #region 패킷's
        public const int PACKET_DATA_REGISTER = 1;
        public const int PACKET_DATA_REGISTER_ACK = 2;

        public const int PACKET_DATA_LOGIN = 3;
        public const int PACKET_DATA_LOGIN_ACK = 4;
        #endregion

        #region 클라이언트 --> 서버
        public static string MakeRegister(string id, string pw, string name, string age, string phone)
        {
            string packet = PACKET_DATA_REGISTER + "@";

            packet += id + "#";
            packet += pw + "#";
            packet += name + "#";
            packet += age + "#";
            packet += phone;
            return packet;
        }
        public static string MakeLogin(string id, string pw)
        {
            string packet = PACKET_DATA_LOGIN + "@";
            packet += id + "#";
            packet += pw;
            return packet;
        }   
        #endregion
    }
}

//Packet.cs
using _0424_ChatClient;
using System;
using System.Drawing;

namespace WoosongBit41.ClientNet
{
    internal static class Packet
    {
        public const int PACKET_Client_LOGIN = 1;
        public const int PACKET_MEMBER_LOGIN_ACK = 2;

        public const int PACKET_MEMBER_LOGOUT = 3;
        public const int PACKET_MEMBER_LOGOUT_ACK = 4;

        public const int PACKET_MEMBER_ADDMEMBER = 5;
        public const int PACKET_MEMBER_ADDMEMBER_ACK = 6;

        public const int PACKET_CHAT_SENDMESSAGE = 7;
        public const int PACKET_CHAT_SENDMESSAGE_ACK = 8;

        #region Clinet -> Server
        public static string MemberLogIn(string id, string pw)
        {
            string packet = PACKET_Client_LOGIN + "@";

            packet += id + "#";
            packet += pw + "#";
            packet += DateTime.Now;

            return packet;
        }
        public static string MemberLogOut(string id)
        {
            string packet = PACKET_MEMBER_LOGOUT + "@";

            packet += id + "#";
            packet += DateTime.Now;

            return packet;
        }
        public static string MemberAddMember(string id, string pw, string name, Color color, DateTime date)
        {
            string packet = PACKET_MEMBER_ADDMEMBER + "@";

            packet += id + "#";
            packet += pw + "#";
            packet += name + "#";
            packet += color.Name + "#";
            packet += date;

            return packet;
        }
        public static string ChatSendMessage(string id, string msg, DateTime date)
        {
            string packet = PACKET_CHAT_SENDMESSAGE + "@";

            packet += id + "#";
            packet += msg + "#";
            packet += date;

            return packet;
        }
        #endregion

    }
}

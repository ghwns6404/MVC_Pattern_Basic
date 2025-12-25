//Packet.cs
using System;
using System.Collections.Generic;
using WoosongBit41.Data;

namespace WoosongBit41.ServerNet
{
    internal static class Packet
    {
        public const int PACKET_MEMBER_LOGIN = 1;
        public const int PACKET_MEMBER_LOGIN_ACK = 2;

        public const int PACKET_MEMBER_LOGOUT = 3;
        public const int PACKET_MEMBER_LOGOUT_ACK = 4;

        public const int PACKET_MEMBER_ADDMEMBER = 5;
        public const int PACKET_MEMBER_ADDMEMBER_ACK = 6;

        public const int PACKET_CHAT_SENDMESSAGE = 7;
        public const int PACKET_CHAT_SENDMESSAGE_ACK = 8;

        #region Server -> Client
        public static string MemberLogIn(bool ischeck, string id, string name)
        {
            string packet = PACKET_MEMBER_LOGIN_ACK + "@";

            packet += ischeck + "#";
            packet += id + "#";
            packet += name;

            return packet;
        }
        public static string MemberLogOut(bool ischeck, string id, string name)
        {
            string packet = PACKET_MEMBER_LOGOUT_ACK + "@";

            packet += ischeck + "#";
            packet += id + "#";
            packet += name;

            return packet;
        }
        public static string MemberAddMember(bool ischeck, string id, string name)
        {
            string packet = PACKET_MEMBER_ADDMEMBER_ACK + "@";

            packet += ischeck + "#";
            packet += id + "#";
            packet += name;

            return packet;
        }
        #endregion
    }
}

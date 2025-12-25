//Packet.cs
using System;
using System.Collections.Generic;
using WoosongBit41.Data;

namespace WoosongBit41.ServerNet
{
    internal static class Packet
    {
        public const int PACKET_INSERT_ACCOUNT          = 1;
        public const int PACKET_INSERT_ACCOUNT_ACK      = 2;
        public const int PACKET_SELECT_ACCOUNT          = 3;
        public const int PACKET_SELECT_ACCOUNT_ACK      = 4;
        public const int PACKET_INPUT_ACCOUNT           = 5;
        public const int PACKET_INPUT_ACCOUNT_ACK       = 6;
        public const int PACKET_OUTPUT_ACCOUNT          = 7;
        public const int PACKET_OUTPUT_ACCOUNT_ACK      = 8;
        public const int PACKET_DELETE_ACCOUNT          = 9;
        public const int PACKET_DELETE_ACCOUNT_ACK      = 10;
        public const int PACKET_PRINTALL_ACCOUNT        = 11;
        public const int PACKET_PRINTALL_ACCOUNT_ACK    = 12;
        public const int PACKET_INPUTOUTPUT_ACCOUNT     = 13;
        public const int PACKET_INPUTOUTPUT_ACCOUNT_ACK = 14;

        #region Server -> Client
        public static string InsertAccount(bool ischeck, string info, int number)
        {
            string packet = PACKET_INSERT_ACCOUNT_ACK + "@";

            packet += ischeck + "#";
            packet += info + "#";
            packet += number;

            return packet;
        }
        public static string SelectAccount(bool ischeck, string info, Account acc)
        {
            string packet = PACKET_SELECT_ACCOUNT_ACK + "@";

            packet += ischeck + "#";
            packet += info + "#";
            packet += acc.Number + "#";
            packet += acc.Name + "#";
            packet += acc.Balance + "#";
            packet += acc.Ctime;

            return packet;
        }
        public static string InputAccount(bool ischeck, string info, int money)
        {
            string packet = PACKET_INPUT_ACCOUNT_ACK + "@";

            packet += ischeck + "#";
            packet += info + "#";
            packet += money;

            return packet;
        }
        public static string OutputAccount(bool ischeck, string info, int money)
        {
            string packet = PACKET_OUTPUT_ACCOUNT_ACK + "@";

            packet += ischeck + "#";
            packet += info + "#";
            packet += money;

            return packet;
        }
        public static string DeleteAccount(bool ischeck, string info, int number)
        {
            string packet = PACKET_DELETE_ACCOUNT_ACK + "@";

            packet += ischeck + "#";
            packet += info + "#";
            packet += number;

            return packet;
        }
        public static string InputOutputAccount(bool ischeck, string info, int money)
        {
            string packet = PACKET_INPUTOUTPUT_ACCOUNT_ACK + "@";

            packet += ischeck + "#";
            packet += info + "#";
            packet += money;

            return packet;
        }
        public static string PrintAllAccount(bool ischeck, string info, List<Account> accounts)
        {
            string packet = PACKET_PRINTALL_ACCOUNT_ACK + "@";

            foreach (Account acc in accounts)
            {
                packet += acc.Number + "#";
                packet += acc.Name + "#";
                packet += acc.Balance + "#";
                packet += acc.Ctime + "$";
            }
            packet.TrimEnd('$');
            return packet;
        }        
        #endregion
    }
}

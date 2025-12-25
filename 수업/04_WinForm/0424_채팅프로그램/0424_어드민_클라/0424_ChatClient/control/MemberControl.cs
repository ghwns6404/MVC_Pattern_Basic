using System;
using System.Data;
using System.Net.Sockets;


namespace _0424_ChatClient
{
    internal class MemberControl
    {
      private MyDB db = new MyDB();

        #region 싱글톤 패턴
        public static MemberControl Singleton { get; } = null;
        static MemberControl() { Singleton = new MemberControl(); }
        private MemberControl()
        {
        }
        #endregion
        
        public DataTable GetMemberTable()
        {
            return db.GetMemberTable();
        }
        
    }
}

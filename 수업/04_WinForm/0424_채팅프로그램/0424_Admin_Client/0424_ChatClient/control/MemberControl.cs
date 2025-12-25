using _0415_데이터베이스;
using System;
using System.Data;
using System.Drawing;
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

        #region 시작과 종료 메서드
        public DataTable GetMemberTable()
        {
           return db.GetMember_Table();
        }
        #endregion
    }
}

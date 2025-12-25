//Account.cs
using System;
using System.Drawing;
using WoosongBit41.Lib;

namespace WoosongBit41.Data
{
    internal class Member
    {
        #region 1. 맴버필드, 프로퍼티(속성)
        public string MemberID { get; set; }
        public string MemberPW { get; set; }
        public string MemberName { get;  set; }
        public Color MemberColor { get;  set; }
        public DateTime MemberTime { get;  set; }
        #endregion

        #region 2. 생성자
        public Member(string id, string pw, string name, Color color, DateTime date)
        {
            MemberID    = id;
            MemberPW    = pw;
            MemberName  = name;
            MemberColor = color;
            MemberTime  = date;
        }
        public Member(string name)
        {
            MemberName = name;
        }
        #endregion
    }
}

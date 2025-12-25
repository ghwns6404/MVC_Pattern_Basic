using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace _0418_DB평가
{
    internal class Subject
    {
        #region 1. 맴버필드, 프로퍼티(속성)
        public int SubId { get; private set; }
        public string SubName { get; private set; }
        #endregion

        #region 2. 생성자
        public Subject() { }
        public Subject(int _subid, string _subname)
        {
            SubId   = _subid;
            SubName = _subname;
        }

        #endregion

        #region 3. 기능 메서드

        public void Print()
        {
            Console.Write(SubId + "\t");
            Console.Write(SubName);
            Console.WriteLine();
        }
        #endregion
    }
}

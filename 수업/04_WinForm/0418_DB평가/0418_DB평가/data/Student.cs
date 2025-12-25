//Account.cs
using System;
using WoosongBit41.Lib;

namespace WoosongBit41.Data
{
    internal class Student
    {
        #region 1. 맴버필드, 프로퍼티(속성)
        public int StuId        { get; private set; }
        public string StuName   { get; private set; }
        
        #endregion

        #region 2. 생성자
        public Student() { }
        public Student(int _stuid, string _stuname)
        {
            StuId   = _stuid;
            StuName = _stuname;
        }

        #endregion

        #region 3. 기능 메서드
        
        public void Print()
        {
            Console.Write(StuId + "\t");
            Console.Write(StuName);
            Console.WriteLine();
        }      
        #endregion 
    }
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace _0418_DB평가
{
    internal class Jumsu
    {
        #region 1. 맴버필드, 프로퍼티(속성)
        public int JumsuId { get; private set; }
        public int StuId { get; private set; }
        public int SubId { get; private set; }
        public int JumsuNum { get; private set; }
        #endregion

        #region 2. 생성자
        public Jumsu() { }
        public Jumsu(int _jumsuid, int _stuid, int _subid, int jumsunum)
        {
            JumsuId     = _jumsuid;
            StuId       = _stuid;
            SubId       = _subid;
            JumsuNum    = jumsunum;
        }

        #endregion

        #region 3. 기능 메서드

        public void Print()
        {
            Console.Write(JumsuId + "\t");
            Console.Write(StuId + "\t");
            Console.Write(SubId + "\t");
            Console.Write(JumsuNum);
            Console.WriteLine();
        }
        public void Println()
        {
            Console.WriteLine($"인덱스      ->  {JumsuId}");
            Console.WriteLine($"회원 아이디 ->  {StuId}");
            Console.WriteLine($"과목 아이디 ->  {SubId}");
            Console.WriteLine($"과목 점수   ->  {JumsuNum}");
        }
        #endregion
    }
}

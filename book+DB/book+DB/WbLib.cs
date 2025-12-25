using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace book_DB
{
    public class WbLib
    {
        public void Pause()
        {
            Console.WriteLine("\n\n\n엔터키 눌러서 계속");
            Console.ReadKey(true);
        }

        public void Logo()
        {
            Console.Clear();
            Console.WriteLine("------------------------");
            Console.WriteLine("0604 책 관프");
            Console.WriteLine("+ DB");
            Console.WriteLine("------------------------");
            Pause();
        }

        public ConsoleKey Menu()
        {
            Console.WriteLine("\n\n[F1] - 책 저장 - Insert");
            Console.WriteLine("[F2] - 책 검색 - Select");
            Console.WriteLine("[F3] - 책 수정 - Update");
            Console.WriteLine("[F4] - 책 삭제 - Delete");
            Console.WriteLine("[F5] - 책 전체출력 - PrintAll");
            return Console.ReadKey().Key;
        }

        public void Ending()
        {
            Console.WriteLine("\n\n프프로그램 끝");
           
        }
    }
}

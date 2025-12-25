using System;

namespace _0604_test
{
    public class WbLib
    {
        public void Pause()
        {
            Console.WriteLine("\n\n엔터키를 눌러 계속");
            Console.ReadKey(true);
        }

        public void Logo()
        {
            Console.WriteLine("------------------");
            Console.WriteLine("0604");
            Console.WriteLine("관프");
            Console.WriteLine("------------------");
        }
        
        public ConsoleKey Menu()
        {
            Console.WriteLine("[F1] - 데이터 추가");
            Console.WriteLine("[F2] - 데이터 삭제");
            Console.WriteLine("[F3] - 데이터 검색");
            Console.WriteLine("[F4] - 데이터 변경");
            Console.WriteLine("[F5] - 데이터 전체출력\n");
            Console.WriteLine("[ESC] - 프로그램 종료\n");
            return Console.ReadKey().Key;
        }

        public void Ending()
        {
            Console.WriteLine("------------------");
            Console.WriteLine("끝");
            Console.WriteLine("------------------");            
        }
    }
}

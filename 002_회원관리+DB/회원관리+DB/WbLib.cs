//WbLib.cs

using System;


namespace 회원관리_DB
{
    internal class WbLib
    {
        #region 1. 로고, 메뉴, 종료출력메시지
        public static void Pause()
        {
            Console.Write("\n\n엔터키를 누르세요");
            Console.ReadKey(true);
        }

        public static void Logo()
        {
            Console.Clear();
            Console.WriteLine("*************************************************************************");
            Console.WriteLine("회원관리 + Database");
            Console.WriteLine("회원관리 1호");
            Console.WriteLine(" ");
            Console.WriteLine(" 2025-05-3");
            Console.WriteLine(" made by 천호준");
            Console.WriteLine("*************************************************************************");
            Pause();
        }

        public static ConsoleKey MenuPrint()
        {
            Console.Write("*************************************************************************\n");
            Console.WriteLine("[ESC] 프로그램 종료\n");
            Console.WriteLine("[F1] 회원 추가 - Insert");
            Console.WriteLine("[F2] 회원 검색 - Select");
            Console.WriteLine("[F3] 회원 수정 - Update");
            Console.WriteLine("[F4] 회원 삭제 - Delete\n");
            Console.WriteLine("[F5] 회원 전체출력 - PrintAll");
            Console.WriteLine("[F6] 회원 전체삭제 - DeleteAll");
            Console.Write("*************************************************************************\n");
            return Console.ReadKey().Key;
        }

        public static void Ending()
        {
            Console.Clear();
            Console.Write("*************************************************************************\n");
            Console.WriteLine("프로그램을 종료합니다.");
            Console.Write("*************************************************************************\n");
            Pause();
        }

        #endregion

        #region 2. 입력
        public static int InputInteger(string msg)
        {
            Console.Write(msg + ":");

            return int.Parse(Console.ReadLine());
        }

        public static float InputFloat(string msg)
        {
            Console.Write(msg + ":");

            return float.Parse(Console.ReadLine());
        }

        public static char InputChar(string msg)
        {
            Console.Write(msg + ":");

            return char.Parse(Console.ReadLine());
        }

        public static string InputString(string msg)
        {
            Console.Write(msg + ":");

            return Console.ReadLine();
        }
        #endregion
    }
}

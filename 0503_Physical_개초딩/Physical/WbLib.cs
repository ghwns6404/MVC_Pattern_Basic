using System;

namespace Physical
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
            Console.WriteLine("스태미너 관리 프로그램");
            Console.WriteLine("스태미너 1호");
            Console.WriteLine(" ");
            Console.WriteLine(" 2025-05-3");
            Console.WriteLine(" made by 천호준");
            Console.WriteLine("*************************************************************************");
            Pause();
        }

        public static ConsoleKey MenuPrint()
        {
            Console.Write("*************************************************************************\n");
            Console.WriteLine("[ESC] 프로그램 종료");
            Console.WriteLine("[F1] 걷기 - [스태미너 - 10]");
            Console.WriteLine("[F2] 뛰기 - [스태미너 - 20]");
            Console.WriteLine("[F3] 잠자기 - [스태미너 + 10]");
            Console.WriteLine("[F4] 개발중.....");
            Console.WriteLine("[F5] 개발중.....");
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

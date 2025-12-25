using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

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
            Console.WriteLine("[F1] ");
            Console.WriteLine("[F2] ");
            Console.WriteLine("[F3] ");
            Console.WriteLine("[F4] ");
            Console.WriteLine("[F5] ");
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

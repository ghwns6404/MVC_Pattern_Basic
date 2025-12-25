using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Drink_last
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
            Console.WriteLine("물관리 ㅋㅋ");
            Console.WriteLine("물관리 1호");
            Console.WriteLine(" ");
            Console.WriteLine(" 2025-05-2");
            Console.WriteLine(" made by 천호준");
            Console.WriteLine("*************************************************************************");
            Pause();
        }

        public static ConsoleKey MenuPrint()
        {
            Console.Write("*************************************************************************\n");
            Console.WriteLine("[ESC] 프로그램 종료");
            Console.WriteLine("[F1] 물 먹기");
            Console.WriteLine("[F2] 물 뱉기");
            Console.WriteLine("[F3] 누가 물을 따라주는데 얼만큼인지는 모르는 랜덤 물마시기");
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

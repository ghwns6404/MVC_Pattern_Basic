//App.cs
using System;
using WoosongBit41.Data;
using WoosongBit41.Lib;

namespace _0418_DB평가
{
    internal class App
    {
        private JumsuControl jumsu_control = JumsuControl.Singleton;

        #region 0. 싱글톤 패턴
        public static App Singleton { get; } = null;
        static App() { Singleton = new App(); }
        private App() {}
        #endregion

        public void Init()
        {
            jumsu_control.Init();
            WbLib.Logo();
        }

        public void Run()
        {
            while (true)
            {
                Console.Clear();
                jumsu_control.PrintAll();
                switch (WbLib.MenuPrint())
                {
                    case ConsoleKey.Escape: return; 
                    case ConsoleKey.F1: jumsu_control.Jumsu_Insert();       break;
                    case ConsoleKey.F2: jumsu_control.Jumsu_Select();       break;
                    case ConsoleKey.F3: jumsu_control.Jumsu_Update();       break;
                    case ConsoleKey.F4: jumsu_control.Jumsu_Delete();       break;
                    case ConsoleKey.F5: jumsu_control.Jumsu_Select_Join();  break;
                    case ConsoleKey.F6: jumsu_control.Jumsu_Max();          break;
                    default: Console.WriteLine("잘못 입력하셨습니다.");     break;
                }
                WbLib.Pause();
            }
        }

        public void Exit()
        {
            jumsu_control.Exit();
            WbLib.Ending();
        }
    }
}

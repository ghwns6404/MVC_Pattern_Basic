//App.cs
using System;

namespace 회원관리_DB
{
    internal class App
    {
        #region 싱글톤
        public static App Singleton { get; } = null;
        static App() { Singleton = new App(); }
        private App()
        {

        }

        #endregion
        private MemberControl membercontrol = MemberControl.Singleton;
        private MyDB db = new MyDB();

        public void Init()
        {
            membercontrol.DB_Connect();
            WbLib.Logo();
        }
        public void Run()
        {
            while (true)
            {
                Console.Clear();

                switch (WbLib.MenuPrint())
                {
                    case ConsoleKey.Escape: return;
                    case ConsoleKey.F1: membercontrol.Control_Insert(); break;
                    case ConsoleKey.F2: membercontrol.Control_Select(); break;
                    case ConsoleKey.F3: membercontrol.Control_Update(); break;
                    case ConsoleKey.F4: membercontrol.Control_Delete(); break;
                    case ConsoleKey.F5: membercontrol.Control_PrintAll(); break;
                    case ConsoleKey.F6: membercontrol.Control_DeleteAll(); break;

                    default: Console.WriteLine("잘못 입력하셨습니다."); break;
                }
                WbLib.Pause();
            }
        }
        public void Exit()
        {
            membercontrol.DB_Close();
            WbLib.Ending();
        }
    }
}

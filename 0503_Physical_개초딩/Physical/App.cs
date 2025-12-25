using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace Physical
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

        public StaminaControl statminacontrol = new StaminaControl();
        public void Init()
        {
            WbLib.Logo();
        }
        public void Run()
        {
            while (true)
            {
                Console.Clear();
               
                Console.WriteLine($"현재 나의 체력: {statminacontrol.human.State()}\n");
                Console.WriteLine("↑↑위 체력이 부족하면 잠을 먼저 자세요 !!");
                
                switch (WbLib.MenuPrint())
                {
                    case ConsoleKey.Escape: return;
                    case ConsoleKey.F1: statminacontrol.Control_Walk(); break; 
                    case ConsoleKey.F2: statminacontrol.Control_Run(); break; 
                    case ConsoleKey.F3: statminacontrol.Control_Sleep(); break; 

                    default: Console.WriteLine("잘못 입력하셨습니다."); break;
                }
                WbLib.Pause();
            }
        }
        public void Exit()
        {
            WbLib.Ending();
        }
    }
}

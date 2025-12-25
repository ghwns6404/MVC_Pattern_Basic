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
        
        public void Init()
        {
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
                    case ConsoleKey.F1:  break; 
                    case ConsoleKey.F2:  break; 
                    case ConsoleKey.F3:  break; 

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

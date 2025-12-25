using System;
using WoosongBit41.Lib;

namespace _0421_shop
{
    internal class App
    {
        private ShopControl shop_control = ShopControl.Singleton;

        #region 0. 싱글톤 패턴
        public static App Singleton { get; } = null;
        static App() { Singleton = new App(); }
        private App() { }
        #endregion

        public void Init()
        {
            shop_control.Init();
            WbLib.Logo();
        }

        public void Run()
        {
            while (true)
            {
                Console.Clear();
                //shop_control.Println();
                switch (WbLib.MenuPrint())
                {
                    case ConsoleKey.Escape: return;
                    case ConsoleKey.F1: shop_control.Insert(); break;
                    case ConsoleKey.F2: shop_control.PrintAll(); break;
                    case ConsoleKey.F3: shop_control.Print_Category(); break;
                    case ConsoleKey.F4: shop_control.Select_Category(); break;
                    case ConsoleKey.F5: shop_control.Select_Name(); break;
                    case ConsoleKey.F6: shop_control.Select_Max(); break;
                    case ConsoleKey.F7: shop_control.Select_Min(); break;
                    case ConsoleKey.F12: shop_control.Delete(); break;





                    default: Console.WriteLine("잘못 입력하셨습니다."); break;
                }
                WbLib.Pause();
            }
        }

        public void Exit()
        {
            shop_control.Exit();
            WbLib.Ending();
        }
    }
}

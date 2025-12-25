using System;
using System.Threading;


namespace Drink_last
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
        private WaterControl watercontrol = WaterControl.Singleton;
        public void Init()
        {
            WbLib.Logo();
        }
        public void Run()
        {
            while (true)
            {
                Console.Clear();
                Console.WriteLine($"현재 나의 물 양: {watercontrol.human.GetMoisture()}");
                if (watercontrol.last_moisture > 300)
                {
                    Console.WriteLine("사망!!!!!!!!!!!");
                    Console.WriteLine("사망!!!!!!!!!!!");
                    Console.WriteLine("사망!!!!!!!!!!!");
                    Console.WriteLine("사망!!!!!!!!!!!");
                    Thread.Sleep(2000);
                    break;
                }

                switch (WbLib.MenuPrint())
                {
                    case ConsoleKey.Escape: return;
                    case ConsoleKey.F1: watercontrol.Control_Drink(); break; //물먹기
                    case ConsoleKey.F2: watercontrol.Control_Spit(); break; //뱉기
                    case ConsoleKey.F3: watercontrol.Control_Random(); break; //랜덤한 양을 먹기

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

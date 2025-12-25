//WaterControl.cs
using System;


namespace Drink_last
{
    public class WaterControl //오로지 컨트롤만 담당
    {
        public Human human = new Human(0);
        public WaterCup watercup = new WaterCup();

        public int last_moisture;

        #region 싱글톤
        public static WaterControl Singleton { get; } = null;
        static WaterControl() { Singleton = new WaterControl(); }
        private WaterControl() { }
        #endregion
        public void Control_Drink()
        {
            human.Drink();

            human.Judge_Water();
        }
        public void Control_Spit()
        {
            int spit_ml = WbLib.InputInteger("뱉을 물");

            human.Spit(spit_ml);

        }
        public void Control_Random()
        {
            last_moisture = human.Random_Drink();

            if (last_moisture > 60 && last_moisture <= 100)
            {
                Console.WriteLine($"현재 수분{last_moisture}이므로 [목이 안마름]");
            }
            else if (last_moisture < 60 && last_moisture >= 0)
            {
                Console.WriteLine($"현재 수분{last_moisture}이므로 [목이 마름] ");
            }
            else
            {
                Console.WriteLine("\n물고문중");
            }
        }
    }
}

using System;

namespace Drink_last
{
    public class WaterCup
    {
        public int cup;
        public WaterCup()
        {
            cup = 0;
        }
        public int Fill()
        {
            //사용자가 마신 양 + 현재 수분값
            //return
            int ml = InputInteger("얼마나 마실건데");
            this.cup = ml;

            if (this.cup > 0 && this.cup <= 100)
                return cup;
            else
                return -1;
        }
        public static int InputInteger(string msg)
        {
            Console.Write(msg + " :");
            return int.Parse(Console.ReadLine());
        }
    }
}

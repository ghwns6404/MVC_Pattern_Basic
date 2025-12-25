//WaterCup.cs
using System;

namespace Drink_last
{
    public class WaterCup
    {
        private int cup;

        public WaterCup()
        {
            cup = 0;
        }
        public int Fill()
        {
            //사용자가 마신 양 + 현재 수분값
            //return
            int ml = WbLib.InputInteger("얼마나 마실건데, (컵 용량:100ml)");
            this.cup = ml;

            if (this.cup > 0 && this.cup <= 100)
                return cup;
            else
                return -1;
        }
        public int Random_Fill()
        {
            Random rand = new Random();
            int Random_ml = rand.Next(1, 101); 
            Console.WriteLine("누가 막 따라준게 이정도야: " + Random_ml);

            if (Random_ml > 0 && Random_ml <= 100)
                return Random_ml;
            else
                return -1;
        }
    }
}

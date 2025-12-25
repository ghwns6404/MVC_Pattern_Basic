//Human.cs
/*
main
human  채워진 물을 마심, 근데 비율에 따라 목이 마른지 안마른지 결정
cup 물을 얼만큼 채울지 

 */

using System;

namespace Drink_last
{
    public class Human
    {
        public WaterCup watercup = new WaterCup();

        private int moisture = 0;     //사람의 수분양
 
        public Human(int moisture)
        {
            this.moisture = moisture;
        }
        //watercup에서 채워진 물을 마심
        public int Drink()
        {
            int cup = watercup.Fill();
            if (cup == -1)
            {
                Console.WriteLine("그정도의 물은 담을 수 없어");
                return -1;
            }
            else
            {
                moisture += cup;
                return moisture;
            }
        }
        public void Spit(int spit_ml)
        {
            if (spit_ml > 0 && spit_ml <= 100)
            {
                if (spit_ml <= moisture)
                {
                    moisture -= spit_ml;
                    Console.WriteLine($"뱉은 물{spit_ml} 로 인해 남은 물 {moisture}");
                }
                else
                {
                    Console.WriteLine("그만큼은 뱉을 수 없어 !!!");
                    return;
                }
            }
            else
            {
                Console.WriteLine("양수만 입력!!");
                return;
            }
        }
        public void Judge_Water()
        {
            if (moisture >= 60 && moisture <= 100)
            {
                Console.WriteLine($"현재 수분{moisture}이므로 [목이 안마름]");
            }
            else if (moisture < 60 && moisture >= 0)
            {
                Console.WriteLine($"현재 수분{moisture}이므로 [목이 마름] ");
            }
            else
            {
                Console.WriteLine("\n컵이 넘쳐흘러 물고문중");
            }
        }
        public int Random_Drink()
        {
            int Random_ml = watercup.Random_Fill();
            if (Random_ml == -1)
            {
                return -1;
            }
            else
            {
                moisture += Random_ml;
                return moisture;
            }
        }
        public int GetMoisture()
        {
            return moisture;
        }
    }
}

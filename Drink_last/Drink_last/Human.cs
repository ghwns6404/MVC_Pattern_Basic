using System;
using System.Collections.Generic;
using System.Data.SqlTypes;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

/*
main
human  채워진 물을 마심, 근데 비율에 따라 목이 마른지 안마른지 결정
cup 물을 얼만큼 채울지 

 */

namespace Drink_last
{
    public class Human
    {
        private WaterCup watercup = new WaterCup();
        private bool thirsty;

        private int moisture = 0;
        //public Human(bool thirsty, int moisture)
        //{
        //    this.thirsty = thirsty;
        //    this.moisture = moisture;
        //}
        public Human(int moisture)
        {
            this.moisture = moisture;
        }
        //watercup에서 채워진 물을 마심
        public int Drink()
        {
            int cup = watercup.Fill();
            moisture += cup;
            return moisture;
        }
        
        public void Result()
        {
            string state = thirsty ? "목이 마릅니다" : "목이 안말라안말라안말라";
            Console.WriteLine($"현재 수분량 :{moisture}---->{thirsty}");
        }
    }
}

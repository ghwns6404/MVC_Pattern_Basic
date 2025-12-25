using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Remoting.Messaging;
using System.Text;
using System.Threading.Tasks;

namespace Drink_last
{
    internal class Program
    {
       
        static void Main(string[] args)
        {
            WaterCup watercup = new WaterCup();
            Human human = new Human(0);

            while(true)
            {
                int last_moisture = human.Drink();

                if (last_moisture > 60 && last_moisture <= 100)
                {
                    Console.WriteLine($"현재 수분{last_moisture}, 목이 안마름");
                    break;
                }
                else
                {
                    Console.WriteLine($"현재 수분{last_moisture}, 목이 마름");
                }
            }
        }
       
    }
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace _0414_람다식
{
    internal class Program2
    {
        static void Main(string[] args)
        {
            List<int> list = new List<int> { 3, 1, 4, 5, 2 };

            foreach (var item in list)
            {
                Console.WriteLine(item + " * 2 == " + (item * 2));
            }
            Console.WriteLine();

            list.ForEach(
                (elem) => { Console.WriteLine(elem + " * 2 == " + (elem * 2)); }
            );
            Console.WriteLine();

            // 또는 Array.ForEach로 나타낼 수도 있고
            Array.ForEach(list.ToArray(), (elem) => { Console.WriteLine(elem + " * 2 == " + (elem * 2)); });
            Console.WriteLine();

            // 또는 람다 식이 아닌 익명 메서드로도 나타낼 수 있음
            list.ForEach(delegate (int elem) { Console.WriteLine(elem + " * 2 == " + (elem * 2)); });

        }
    }
}

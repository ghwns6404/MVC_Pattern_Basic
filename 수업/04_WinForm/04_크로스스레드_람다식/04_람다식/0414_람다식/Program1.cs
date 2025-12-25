using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace _0414_람다식
{
    internal class Program1
    {
        private static List<int> FilterOfInts(int[] source, Func<int, bool> filter)
        {
            List<int> result = new List<int>();
            foreach (int i in source)
            {
                if (filter(i))
                    result.Add(i);
            }
            return result;
        }

        static void Main(string[] args)
        {
            int[] source = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

            //===홀수 Filter===
            List<int> oddNumbers = FilterOfInts(source, (i) => ((i & 1) == 1));

            Console.WriteLine("=== 홀수 Filter ===");
            foreach (var item in oddNumbers)
            {
                Console.Write(item);
            }
            Console.WriteLine();

            //===짝수 Filter===
            List<int> evenNumbers = FilterOfInts(source, i => ((i & 1) == 0));

            Console.WriteLine("=== 짝수 Filter ===");
            foreach (var item in evenNumbers)
            {
                Console.Write(item);
            }
            Console.WriteLine();

            //===소수 Filter===
            List<int> primeNumbers = FilterOfInts(source,
            checkNumber =>
            {
                BitArray numbers = new BitArray(checkNumber + 1, true);
                for (int i = 2; i < checkNumber + 1; i++)
                {
                    if (numbers[i])
                    {
                        for (int j = i * 2; j < checkNumber + 1; j += i) { numbers[j] = false; }

                        if (numbers[i]) { if (checkNumber == i) { return true; } }
                    }
                }
                return false;
            });

            Console.WriteLine("=== 소수 Filter ===");
            foreach (var item in primeNumbers)
            {
                Console.Write(item);
            }
            Console.WriteLine();
        }
    }
}

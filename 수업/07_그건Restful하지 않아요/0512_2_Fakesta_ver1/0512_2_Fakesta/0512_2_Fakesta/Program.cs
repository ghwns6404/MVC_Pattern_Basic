// Program.cs

using System.ServiceModel;
using System;
using _0512_2_Fakesta.Service;

namespace _0512_2_Fakesta
{
    internal class Program//천호준 part1
    {
        private static MyDB mydb = MyDB.Singleton;
        private static void Main(string[] args)
        {
            ServiceHost host = new ServiceHost(typeof(FakestaService));
            mydb.Connect();

            host.Open();

            Console.WriteLine("서버를 종료하려면 아무 키나 누르시오.");
            Console.ReadKey(true);

            host.Close();

            mydb.Close();
        }
    }
}

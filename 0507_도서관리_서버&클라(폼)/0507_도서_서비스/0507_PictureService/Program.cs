using System;
using System.ServiceModel;

namespace _0507_PictureService
{
    internal class Program  //수정본입니다 @!@@@@@@@@@@@@#@!#@!#!@#!@#!@#!@#!@#!@#!@#@!#!@#!@#@!#@!
    {
        private static MyDB mydb = MyDB.Singleton;
        static void Main(string[] args)
        {
            
            ServiceHost host = new ServiceHost(typeof(CBook));

            //db연결
            mydb.Connect();

            host.Open();
            Console.WriteLine("Press Any key to stop the service");
            Console.ReadKey();
            host.Close();

            //db close
            mydb.Close();
        }
    }
}

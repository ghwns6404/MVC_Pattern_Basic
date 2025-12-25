using System;
using System.ServiceModel;


namespace _0509_시험_Service
{
    internal class Program
    {
        private static MyDB mydb = MyDB.Singleton;
        static void Main(string[] args)
        {
            mydb.Connect();
            Console.WriteLine("DB 연결 성공\n");
            Console.WriteLine("기도 시작");

            ServiceHost host = new ServiceHost(typeof(_0509_시험_Service.AccountService));

            //오픈
            host.Open();
            Console.WriteLine("성공 !!!!");
            Console.WriteLine("멈추시려면 엔터를 눌러주세요..");
            Console.ReadLine();

            //서비스
            host.Abort();
            host.Close();

            mydb.Close();
        }
    }
}

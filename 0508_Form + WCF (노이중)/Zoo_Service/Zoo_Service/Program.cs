using System;
using System.Collections.Generic;
using System.Configuration;
using System.Linq;
using System.Runtime.Serialization;
using System.ServiceModel;
using System.Text;
using System.Threading.Tasks;

namespace Zoo_Service
{
    
    internal class Program
    {
        private static MyDB mydb = MyDB.Singleton;
        static void Main(string[] args)
        {
            mydb.Connect();
            ServiceHost host = new ServiceHost(typeof(Zoo_Service.ZooService));

            //오픈
            host.Open();
            Console.WriteLine("성공 !!!!");
            Console.WriteLine("성공 !!!!");
            Console.WriteLine("성공 !!!!");
            Console.WriteLine("성공 !!!!");
            Console.WriteLine("멈추시려면 엔터를 눌러주세요..");
            Console.ReadLine();

            //서비스
            host.Abort();
            host.Close();

            //디비 닫기
            mydb.Close();
        }
    }
}

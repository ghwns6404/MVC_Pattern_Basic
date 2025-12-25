using System;
using System.Collections.Generic;
using System.Configuration;
using System.Linq;
using System.Runtime.CompilerServices;
using System.ServiceModel;
using System.Text;
using System.Threading.Tasks;

namespace _0508_Book_Service
{
    internal class Program
    {
        private static MyDB mydb = MyDB.Singleton;
        static void Main(string[] args)
        {
            
            //디비 연결
            mydb.Connect();

            //Address
            Uri uri = new Uri(ConfigurationManager.AppSettings["addr"]);
            //Contract-> Setting
            //Binding -> App.Config
            ServiceHost host = new ServiceHost(typeof(_0508_Book_Service.BookService), uri);

            //오픈
            host.Open();
            Console.WriteLine("관프 시작. {0}", uri.ToString());
            Console.WriteLine("http://220.90.180.108:9000/GetService");
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

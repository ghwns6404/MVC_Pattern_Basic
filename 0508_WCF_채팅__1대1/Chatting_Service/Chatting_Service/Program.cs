using System;
using System.Configuration;
using System.ServiceModel;


namespace Chatting_Service
{
    internal class Program
    {
        static void Main(string[] args)
        {
            //Address
            Uri uri = new Uri(ConfigurationManager.AppSettings["addr"]);
            //Contract-> Setting
            //Binding -> App.Config
            ServiceHost host = new ServiceHost(typeof(Chatting_Service.ChatService), uri);

            //오픈
            host.Open();
            Console.WriteLine("1대 1 채팅 서비스를 시작합니다. {0}", uri.ToString());
            Console.WriteLine("http://220.90.180.108:9000/GetService");
            Console.WriteLine("멈추시려면 엔터를 눌러주세요..");
            Console.ReadLine();
            //서비스
            host.Abort();
            host.Close();

        }
    }
}

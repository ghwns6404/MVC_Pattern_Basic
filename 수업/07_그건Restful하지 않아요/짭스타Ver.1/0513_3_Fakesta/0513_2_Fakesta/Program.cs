// Program.cs

using System.ServiceModel;
using System;
using _0513_2_Fakesta.Database;
using _0513_2_Fakesta.Service;
using _0513_2_Fakesta.Custom;
using System.ServiceModel.Description;

namespace _0513_2_Fakesta
{
    internal class Program
    {
        private static void Main(string[] args)
        {
            ServiceHost host = new ServiceHost(typeof(FakestaService));
            foreach(ServiceEndpoint ep in host.Description.Endpoints)
                ep.Behaviors.Add(new BehaviorAttribute());

            if (!MyDB.Instance.Init(@"BIT\SQLEXPRESS", "WB41", "aaa", "1234"))
                throw new ApplicationException("데이터베이스 클라이언트 초기화를 실패했습니다.");

            host.Open();

            Console.WriteLine("서버를 종료하려면 아무 키나 누르시오.");
            Console.ReadKey(true);

            host.Close();
            MyDB.Instance.Close();
        }
    }
}

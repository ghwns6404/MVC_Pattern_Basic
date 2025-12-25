/*
 -------------------------------
 WCF 서비스 사용 참조 추가 : using System.ServiceModel;
 -------서비스 개발--------------
 1. 인터페이스 : 서비스 계약
 2. 서비스 객체 정의 : 구현 상속
--------------------------------
 3. 호스팅 : exe
  ㄴ WSDL 문서요청 처리
  ㄴ 종점 구성
--------------------------------
 */
using System;
using System.Collections.Generic;
using System.Linq;
using System.ServiceModel;
using System.ServiceModel.Description;
using System.Text;
using System.Threading.Tasks;

namespace _0507_WCFService_1
{

    //호스팅
    internal class Program
    {
        public static void Hosting1()
        {
            //wnth : WSDL문서 요청하는 주소
            ServiceHost host = new ServiceHost(typeof(HelloWorldWCFService),
                              new Uri("http://220.90.180.108/wcf/example/helloworldservice"));

            //주소가 생략되면 servicehost객체에서 찾아서 사용
            host.AddServiceEndpoint(typeof(IHelloWorld),    //service Contract
                                    new BasicHttpBinding(), //service Binding (Web Service)
                                    "");                    //service Address

            //WSDL 문서요청 처리 역할 수행
            ServiceMetadataBehavior behavior = host.Description.Behaviors.Find<ServiceMetadataBehavior>();
            if (behavior == null)
            {
                behavior = new ServiceMetadataBehavior();
                host.Description.Behaviors.Add(behavior);
            }
            behavior.HttpGetEnabled = true;



            host.Open();    //클라이언트 호출 수신
            Console.WriteLine("Press Any key to stop the service");
            Console.ReadKey(true);
            host.Close();   //호출 종료

        }

        //config파일 활용
        public static void Hosting2()
        {
            ServiceHost host = new ServiceHost(typeof(HelloWorldWCFService));

            host.Open();    
            Console.WriteLine("Press Any key to stop the service");
            Console.ReadKey(true);
            host.Close(); 

        }


        static void Main(string[] args)
        {
            Hosting2();
        }
    }
}

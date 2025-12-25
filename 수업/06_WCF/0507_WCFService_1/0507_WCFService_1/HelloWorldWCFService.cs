using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace _0507_WCFService_1
{
    //서비스 객체 (꼭 상속을 받아야 함 !!)
    public class HelloWorldWCFService : IHelloWorld
    {
        public int Number1 { get; set; } = 0;
        public int Number2 { get; set; } = 0;

        public string SayHello()
        {
            return "Hello WCF World";
        }

        public string SayHello1()
        {
            return "hello worrrld";
        }
        public int GetSumNumber()
        {
            return Number1 + Number2;
        }
        public void setNumber(int num1, int num2)
        {
            Number1 = num1;
            Number2 = num2;

        }
    }
}

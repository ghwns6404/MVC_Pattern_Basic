using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;  
using System.Threading.Tasks;
using System.ServiceModel;


namespace _0507_WCFService_1
{
    [ServiceContract]
    public interface IHelloWorld    //인터페이스로 계약
    {
        [OperationContract]
        string SayHello();

        string SayHello1();

        [OperationContract]
        void setNumber(int num1, int num2);

        [OperationContract]
        int GetSumNumber();


    }
}

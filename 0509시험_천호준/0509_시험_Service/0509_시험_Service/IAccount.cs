using System;
using System.Collections.Generic;
using System.Linq;
using System.ServiceModel;
using System.Text;
using System.Threading.Tasks;
using static _0509_시험_Service.IAccount;

namespace _0509_시험_Service
{
    [ServiceContract]
    public interface IAccount
    {
        [OperationContract(IsOneWay = false)]
        bool InsertAccount(int id, string name, int balance);

        [OperationContract(IsOneWay = false)]
        bool InputMoney(int id, int money);

        [OperationContract(IsOneWay = true)]
        void DeleteAccount(int id);

        [OperationContract(IsOneWay = false)]
        Account SelectAccount(int id);

        [OperationContract(IsOneWay = false)]
        List<Account> SelectAllAccount();

    }
}


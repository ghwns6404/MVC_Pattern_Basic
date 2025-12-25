using System;
using System.Collections.Generic;
using System.Linq;
using System.ServiceModel;
using System.Text;
using System.Threading.Tasks;

namespace _0508_Book_Service
{
    [ServiceContract(CallbackContract = typeof(IBookCallback))]
    public interface IBook
    {
        [OperationContract(IsOneWay = false)]
        bool Insert(int id, string title, string author, int price);

        [OperationContract(IsOneWay = true)]
        void Delete(int id);

        [OperationContract(IsOneWay = false)]
        List<Book> Select(int id);

        [OperationContract(IsOneWay = false)]
        bool Update(int id, string title, string author, int price);

        [OperationContract(IsOneWay = false)]
        List<Book> Select_All();
    }

    public interface IBookCallback
    {
        [OperationContract(IsOneWay = true)]
        void Notify_Print_All(string[] return_All);
    }
}

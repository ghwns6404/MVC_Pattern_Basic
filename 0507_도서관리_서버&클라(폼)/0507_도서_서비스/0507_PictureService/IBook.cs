//IPicture.cs

using System.Collections.Generic;
using System.ServiceModel;


namespace _0507_PictureService
{
    [ServiceContract]
    internal interface IBook
    {
        [OperationContract]
        bool C_Insert(int isbn, string title, string author, string publisher, int price);

        [OperationContract]
        bool C_Update(int isbn, string title, string author, string publisher, int price);

        [OperationContract]
        string C_Select_Isbn(int isbn);

        [OperationContract]
        bool C_Delete(int isbn);

        [OperationContract]
        List<Book> C_Select_All();


    }
}

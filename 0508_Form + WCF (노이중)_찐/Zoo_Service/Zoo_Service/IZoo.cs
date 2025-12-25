//IZoo.cs
using System.Collections.Generic;
using System.ServiceModel;


namespace Zoo_Service
{
    [ServiceContract]
    public interface IZoo
    {
        [OperationContract(IsOneWay = false)]
        bool Insert(int id, string name, string country, int age);

        [OperationContract(IsOneWay = true)]
        void Delete(int id);

        [OperationContract(IsOneWay = false)]
        Zoo Select(int id);

        [OperationContract(IsOneWay = false)]
        bool Update(int id, string name, string country, int age);

        [OperationContract(IsOneWay = false)]
        List<Zoo> Select_All();
    }
}

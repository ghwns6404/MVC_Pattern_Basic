using System;
using System.Collections.Generic;
using System.Linq;
using System.ServiceModel;
using System.Text;
using System.Threading.Tasks;

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
        List<Zoo> Select(int id);

        [OperationContract(IsOneWay = false)]
        bool Update(int id, string name, string country, int age);

        [OperationContract(IsOneWay = false)]
        List<Zoo> Select_All();
    }
}

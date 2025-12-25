using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.Text;
using System.Threading.Tasks;

namespace _0509_시험_Service
{
    [DataContract]
    public class Account
    {
        [DataMember]
        public int Id { get; set; }

        [DataMember]
        public string Name { get; set; }

        [DataMember]
        public int Balance { get; set; }

        

        #region 생성자
        public Account()
        {

        }
        public Account(int id, string name, int balance)
        {
            Id = id;
            Name = name;
            Balance = balance;
        }


        #endregion
    }
}

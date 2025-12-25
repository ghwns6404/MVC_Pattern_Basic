using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.Security.Policy;
using System.Text;
using System.Threading.Tasks;

namespace Zoo_Service
{
    [DataContract]
    public class Zoo
    {
        [DataMember]
        public int Id { get; set; }

        [DataMember]
        public string Name { get; set; }

        [DataMember]
        public string Country { get; set; }

        [DataMember]
        public int Age { get; set; }

        public Zoo()
        {

        }
        public Zoo(int id, string name, string country, int age)
        {
            Id = id;
            Name = name;
            Country = country;
            Age = age;
        }
    }
}

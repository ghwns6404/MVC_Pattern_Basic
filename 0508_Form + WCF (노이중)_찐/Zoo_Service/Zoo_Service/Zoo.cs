//Zoo.cs

using System.Runtime.Serialization;


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

        #region 생성자
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
        public Zoo(string name, string country, int age)
        { 
            Name = name;
            Country = country;
            Age = age;
        }
        #endregion
    }
}

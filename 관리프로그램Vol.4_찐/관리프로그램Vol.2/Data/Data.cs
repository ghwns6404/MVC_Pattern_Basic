using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace 관리프로그램Vol._2
{
    public class Data
    {
        public string Id { get; set; }
        public string Pw { get; set; }
        public string Name { get; set; }
        public string Age { get; set; }
        public string Phone { get; set; }
        public Data(string id, string pw, string name, string age, string phone)
        {
            Id = id;
            Pw = pw;
            Name = name;
            Age = age;
            Phone = phone;
        }
        //public Data(string id, int pw)
        //{
        //    Id = id;
        //    Pw = pw;
        //}
        public override string ToString()
        {
            return $"{Id} {Pw} {Name} {Age} {Phone}";
        }
    }
}

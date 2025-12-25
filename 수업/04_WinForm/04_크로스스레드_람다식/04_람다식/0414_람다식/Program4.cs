using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace _0414_람다식
{
    internal class Member
    {
        public string Name { get; private set; }
        public string Phone { get; set; }
        public DateTime DateTime { get; private set; }

        public Member(string name, string phone)
        {

            Name = name;
            Phone = phone;
            DateTime = DateTime.Now;
        }
        public void PrintData()
        {
            Console.WriteLine(Name + ", " + Phone + ", " + DateTime.ToString());
        }
    }

    internal class Person
    {
        public string Name { get; set; }
        public DateTime DateTime { get; set; }     
    }

    internal class Program4
    {
        List<Member> members = new List<Member>();

        public void InitData()
        {
            members.Add(new Member("홍길동", "010-1111-1111"));
            members.Add(new Member("이길동", "010-2222-1111"));
            members.Add(new Member("고길동", "010-3333-1111"));
            members.Add(new Member("공길동", "010-4444-1111"));
            members.Add(new Member("홍길동", "010-1111-1111"));
            members.Add(new Member("홍길동", "010-1111-1111"));
        }
        public void GetData()
        {

            //객체 반환
            IEnumerable<Person> personList = members.Select(
               (elem) => new Person { Name = elem.Name, DateTime = elem.DateTime });
            
            Array.ForEach(personList.ToArray(), (elem) => { Console.WriteLine(elem.Name); });

            Console.WriteLine();
        }       

        static void Main(string[] args)
        {
            Program4 p = new Program4();
            p.InitData();
            p.GetData();


        }
    }
}

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

    internal class Program3
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

        public void PrintAll()
        {
            members.ForEach((elem) => { elem.PrintData(); });
        }

        public void FindMember(string name)
        {
            var evenList = members.FindAll((elem) => elem.Name==name);

            Console.WriteLine("결과");
            evenList.ForEach((elem) => { elem.PrintData(); });
        }

        public void SelectCount(string name)
        {
            Console.WriteLine(name + "이름을 갖고 있는 사람의 수");
            int count = members.Count((elem) => elem.Name ==name);
            Console.WriteLine(count + "개 있음.");
        }

        static void Main(string[] args)
        {
            Program3 p = new Program3();
            p.InitData();
            p.PrintAll();
            p.FindMember("홍길동");
            p.SelectCount("홍길동");

        }
    }
}

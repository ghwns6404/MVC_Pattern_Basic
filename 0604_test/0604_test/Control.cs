using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace _0604_test
{
    public class Control
    {
        public List<Member> member_list = new List<Member>();
        public WbLib lib = new WbLib();
        public Member member = new Member();
        public void Insert()
        {
            try
            {
                Console.WriteLine("데이터를 입력하세요\n");

                Console.Write("이름 : ");
                string name = Console.ReadLine();

                Console.Write("번호 : ");
                string phone = Console.ReadLine();

                Console.Write("나이 : ");
                int age = int.Parse(Console.ReadLine());

                member_list.Add(new Member(name, phone, age));
                Console.WriteLine("\n데이터 저장 완료\n");
            }
            catch
            {
                Console.WriteLine("예외발생");
            }
            lib.Pause();

        }

        public void Select()
        {
            Console.WriteLine("검색할 데이터의 이름을 입력하세요\n");

            Console.Write("이름 : ");
            string name = Console.ReadLine();
            
            if (member_list.Count == 0)
            {
                Console.WriteLine("저장된 데이터가 없ㅅ다");
            }
            try
            {
                foreach (Member mem in member_list)
                {
                    if (mem.Name == name)
                    {
                        Console.WriteLine($"이름:[{mem.Name}]  전화번호:[{mem.Phone}]  나이:[{mem.Age}]\n");
                    }
                }
            }
            catch
            {
                Console.WriteLine("예외발생");
            }
            lib.Pause();
        }

        public void Delete()
        {
            Console.WriteLine("삭제할 데이터의 이름을 입력하세요\n");

            Console.Write("이름 : ");
            string name = Console.ReadLine();

            try
            {
                foreach (Member mem in member_list)
                {
                    if (mem.Name != name)
                    {
                        Console.WriteLine("그런 데이터는 없다");
                    }
                    if (mem.Name == name)
                    {
                        member_list.Remove(mem);
                        Console.WriteLine("\n데이터가 삭제되었습니다\n");
                    }
                    lib.Pause();
                }
            }
            catch
            {
                Console.WriteLine("예외발생");
            }
        }

        public void Update()
        {
            Console.WriteLine("수정할 데이터의 이름을 입력하세요\n");

            Console.Write("수정할 이름 : ");
            string name = Console.ReadLine();
            Console.WriteLine();

            try
            {
                foreach (Member mem in member_list)
                {
                    if (mem.Name == name)
                    {
                        Console.Write("새 이름 : ");
                        mem.Name = Console.ReadLine();

                        Console.Write("새 번호 : ");
                        mem.Phone = Console.ReadLine();

                        Console.Write("새 나이 : ");
                        mem.Age = int.Parse(Console.ReadLine());

                        Console.WriteLine("\n데이터가 수정되었습니다\n");
                        lib.Pause();

                    }
                }
                
            }
            catch
            {
                Console.WriteLine("예외발생");
            }
        }

        public void PrintAll()
        {
            if (member_list.Count == 0)
            {
                Console.WriteLine("저장된 데이터가 없다");
            }
            foreach(Member mem in member_list)
            {
                Console.WriteLine($"이름:[{mem.Name}]  전화번호:[{mem.Phone}]  나이:[{mem.Age}]\n");
               
            }
            lib.Pause();
        }
    }


}

//Member.cs

namespace 회원관리_DB
{
    public class Member
    {
        public int Id { get; private set; }
        public string Name { get; private set; }
        public int Age { get; private set; }
        public string Phone { get; private set; }
        public string Addr { get; private set; }

        public Member(int id, string name, int age, string phone, string addr)
        {
            Id = id;
            Name = name;
            Age = age;
            Phone = phone;
            Addr = addr;
        }
    }
}

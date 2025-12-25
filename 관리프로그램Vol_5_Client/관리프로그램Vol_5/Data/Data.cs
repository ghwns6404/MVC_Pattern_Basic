//Data.cs

namespace _관리프로그램Vol_5
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
        public Data()
        {

        }
        public override string ToString()
        {
            return $"{Id} {Pw} {Name} {Age} {Phone}";
        }
    }
}

// Member.cs

using System.Runtime.Serialization;

namespace _0512_2_Fakesta.Data
{
    /// <summary>
    /// 회원을 나타냅니다.
    /// </summary>
    /// 왜 비밀번호가 없죠? => 보안 문제가 있기 때문에 민감한 정보 전달은 최소화합니다.
    [DataContract]
    internal class Member
    {
        [DataMember]
        public int Id { get; set; }

        [DataMember]
        public string Name { get; set; }

        [DataMember]
        public string Email { get; set; }

        [DataMember]
        public string Created_at { get; set; }

        [DataMember]
        public string Updated_at { get; set; }

        public Member(int id, string email, string name, string created_at, string updated_at)
        {
            Id = id;
            Email = email;
            Name = name;
            Created_at = created_at;
            Updated_at = updated_at;
        }

        public Member(int id, string email, string name)
        {
            Id = id;
            Email = email;
            Name = name;
        }


        public Member(int id)
        {
            Id = id;
        }

        public Member(string name, string email)
        {
            Name = name;
            Email = email;
        }   
        public Member()
        {

        }
    }
}

// Member.cs

using System.Runtime.Serialization;

namespace _0513_2_Fakesta.Data
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
        public string Email { get; set; }

        [DataMember]
        public string Name { get; set; }

        [DataMember]
        public string CreatedAt { get; set; }

        [DataMember]
        public string UpdatedAt { get; set; }

        public PublicMember ToPublic()
        {
            return new PublicMember()
            {
                Id = Id,
                Name = Name,
                CreatedAt = CreatedAt,
                UpdatedAt = UpdatedAt
            };
        }
    }

    /// <summary>
    /// 회원을 나타냅니다. 불특정 다수에게 응답할 때 사용됩니다. 따라서 민감한 정보를 제외합니다.
    /// </summary>
    [DataContract]
    internal class PublicMember
    {
        [DataMember]
        public int Id { get; set; }

        [DataMember]
        public string Name { get; set; }

        [DataMember]
        public string CreatedAt { get; set; }

        [DataMember]
        public string UpdatedAt { get; set; }
    }
}

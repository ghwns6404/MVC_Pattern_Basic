// Session.cs

using System.Runtime.Serialization;

namespace _0513_2_Fakesta.Data
{
    [DataContract]
    internal class Session
    {
        [DataMember]
        public int Id { get; set; }

        [DataMember]
        public int MemId { get; set; }

        [DataMember]
        public string CreatedAt { get; set; }

        [DataMember]
        public string ExpiresAt { get; set; }
    }

    [DataContract]
    internal class CreateSessionResponse
    {
        [DataMember]
        public string Token { get; set; }
    }
}

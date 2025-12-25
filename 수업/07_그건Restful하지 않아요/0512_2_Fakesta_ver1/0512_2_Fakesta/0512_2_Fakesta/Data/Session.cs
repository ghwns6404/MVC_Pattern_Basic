using System.Runtime.Serialization;

namespace _0512_2_Fakesta.Data
{
    [DataContract]
    internal class CreateSessionResponse
    {
        [DataMember]
        public string Token { get; set; }
    }
}

using System;
using System.Runtime.Serialization;
using System.ServiceModel.Security;

namespace _0512_2_Fakesta.Data
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
        public string UpdatedAt { get; set; }
    }

    [DataContract]
    internal class CreateSessionResponse
    {

        [DataMember]
        public string Token { get; set; }


        public CreateSessionResponse(string token)
        {
            Token = token;  
        }

        public CreateSessionResponse()
        {

        }

    }
}

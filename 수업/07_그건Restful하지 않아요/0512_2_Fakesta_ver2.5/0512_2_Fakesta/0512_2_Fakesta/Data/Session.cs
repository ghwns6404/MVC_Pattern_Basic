using System;
using System.Runtime.Serialization;
using System.ServiceModel.Security;

namespace _0512_2_Fakesta.Data
{
    [DataContract]
    internal class Session
    {
        public Session(int mem_id, string token1, string created_at, string expires_at)
        {
        }

        [DataMember]
        public int Id { get; set; }

        [DataMember]
        public int MemId { get; set; }

        [DataMember]
        public string CreatedAt { get; set; }

        [DataMember]
        public string UpdatedAt { get; set; }

        public Session(int memId, string createdAt, string updatedAt)
        {
            MemId = memId;
            CreatedAt = createdAt;
            UpdatedAt = updatedAt;
        }
        public Session()
        {

        }
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

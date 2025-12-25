// Post.cs

using System.Collections.Generic;
using System.Runtime.Serialization;

namespace _0513_2_Fakesta.Data
{
    [DataContract]
    internal class Post
    {
        [DataMember]
        public int Id { get; set; }

        [DataMember]
        public PublicMember Member { get; set; }

        [DataMember]
        public string Title { get; set; }

        [DataMember]
        public string Content { get; set; }

        [DataMember]
        public string CreatedAt { get; set; }

        [DataMember]
        public string UpdatedAt { get; set; }

        [DataMember]
        public List<int> FileIds { get; set; }
    }
}

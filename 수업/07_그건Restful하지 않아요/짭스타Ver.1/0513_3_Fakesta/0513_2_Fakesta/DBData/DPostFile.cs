// DPostFile.cs

using System;

namespace _0513_2_Fakesta.DBData
{
    internal class DPostFile
    {
        public int Id { get; set; }

        public int PostId { get; set; }

        public string Name { get; set; }

        public DateTime CreatedAt { get; set; }

        public DPostFile(int id, int postId, string name, DateTime createdAt)
        {
            Id = id;
            PostId = postId;
            Name = name;
            CreatedAt = createdAt;
        }
    }
}

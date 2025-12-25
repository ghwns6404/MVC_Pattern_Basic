// DPost.cs

using System;
using System.Collections.Generic;
using System.Linq;
using _0513_2_Fakesta.Data;

namespace _0513_2_Fakesta.DBData
{
    internal class DPost
    {
        public int Id { get; set; }

        public Member Member { get; set; }

        public string Title { get; set; }

        public string Content { get; set; }

        public DateTime CreatedAt { get; set; }

        public DateTime UpdatedAt { get; set; }

        public List<DPostFile> Files { get; set; }

        public DPost(int id, Member member, string title, string content, DateTime createdAt, DateTime updatedAt)
        {
            Id = id;
            Member = member;
            Title = title;
            Content = content;
            CreatedAt = createdAt;
            UpdatedAt = updatedAt;
        }

        public Post ToPost()
        {
            return new Post
            {
                Id = Id,
                Member = Member.ToPublic(),
                Title = Title,
                Content = Content,
                CreatedAt = CreatedAt.ToString("O"),
                UpdatedAt = UpdatedAt.ToString("O"),
                FileIds = Files.Select(f => f.Id).ToList()
            };
        }
    }
}

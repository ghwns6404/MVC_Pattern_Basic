// DSession.cs

using System;
using _0513_2_Fakesta.Data;

namespace _0513_2_Fakesta.DBData
{
    internal class DSession
    {
        public int Id { get; set; }

        public Member Member { get; set; }

        public string Token { get; set; }

        public DateTime CreatedAt { get; set; }

        public DateTime ExpiresAt { get; set; }

        public DSession(int id, Member member, string token, DateTime createdAt, DateTime expiresAt)
        {
            Id = id;
            Member = member;
            Token = token;
            CreatedAt = createdAt;
            ExpiresAt = expiresAt;
        }

        public Session ToSession()
        {
            return new Session
            {
                Id = Id,
                MemId = Member.Id,
                CreatedAt = CreatedAt.ToString("O"),
                ExpiresAt = ExpiresAt.ToString("O")
            };
        }
    }
}

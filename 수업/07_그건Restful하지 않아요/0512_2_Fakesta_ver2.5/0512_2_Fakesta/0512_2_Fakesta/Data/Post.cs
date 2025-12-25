using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.Text;
using System.Threading.Tasks;

namespace _0512_2_Fakesta
{
    [DataContract]
    internal class Post
    {
        [DataMember]
        public int Id { get; set; }

        [DataMember]
        public int MemberId { get; set; }

        [DataMember]
        public string Name { get; set; }

        [DataMember]
        public string CreatedAt { get; set; }

        [DataMember]
        public string UpdatedAt { get; set; }

        [DataMember]
        public List<int> FileIds { get; set; }

        public Post()
        {
            FileIds = new List<int>();
        }
        public Post(int id, int memberId, string name, string createdAt, string updatedAt, List<int> fileIds)
        {
            Id = id;
            MemberId = memberId;
            Name = name;
            CreatedAt = createdAt;
            UpdatedAt = updatedAt;
            FileIds = fileIds;
        }
        public Post(int id, int memberId, string name, string createdAt, string updatedAt, string updated_at)
        {
            Id = id;
            MemberId = memberId;
            Name = name;
            CreatedAt = createdAt;
            UpdatedAt = updatedAt;
            
        }
        public override string ToString()
        {
            return $"Post: {Id}, MemberId: {MemberId}, Title: {Name}, CreatedAt: {CreatedAt}, UpdatedAt: {UpdatedAt}, FileIds: {string.Join(", ", FileIds)}";
        }


        #region 3. 날짜와 시간을 문자열로 반환
        public static string Get_Date(DateTime time)
        {
            return string.Format("{0}-{1}-{2}", time.Year, time.Month, time.Day);
        }
        public static string Get_Time(DateTime time)
        {
            return string.Format("{0}:{1}:{2}", time.Hour, time.Minute, time.Second);
        }

        public static string Get_DateTime(DateTime time)//주로 이거씀
        {
            return string.Format("{0}-{1}-{2} {3}:{4}:{5}", time.Year, time.Month, time.Day, time.Hour, time.Minute, time.Second);
        }


        #endregion
    }
}

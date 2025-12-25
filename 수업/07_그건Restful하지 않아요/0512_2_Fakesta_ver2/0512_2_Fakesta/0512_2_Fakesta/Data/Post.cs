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
        public int Mem_Id { get; set; }

        [DataMember]
        public string Name { get; set; }

        [DataMember]
        public string Content { get; set; }

        [DataMember]
        public DateTime Created_at { get; set; }

        [DataMember]
        public DateTime Updated_at { get; set; }


        public Post(int id, int mem_Id, string name, string Content, DateTime created_at, DateTime updated_at)
        {
            Id = id;
            Mem_Id = mem_Id;
            Name = name;
            this.Content = Content;
            Created_at = created_at;
            Updated_at = updated_at;
        }

        public Post()
        {

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

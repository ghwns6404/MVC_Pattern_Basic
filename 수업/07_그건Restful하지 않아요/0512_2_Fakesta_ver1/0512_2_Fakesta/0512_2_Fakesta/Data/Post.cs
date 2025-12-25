// Post.cs

using System;
using System.Runtime.Serialization;

namespace _0512_2_Fakesta.Data
{
    [DataContract]
    internal class Post
    {
        [DataMember]
        public int Id { get; set; }

        [DataMember]
        public int MemberId { get; set; }

        [DataMember]
        public string Title { get; set; }

        [DataMember]
        public string Filename { get; set; }

        [DataMember]
        public string Path { get; set; }

        [DataMember]
        public DateTime Date { get; set; }


        public Post(int id, int memberId, string title, string filename, string path, DateTime date)
        {
            Id = id;
            MemberId = memberId;
            Title = title;
            Filename = filename;
            Path = path;
            Date = date;
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

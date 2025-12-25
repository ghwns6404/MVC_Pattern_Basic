using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.Text;
using System.Threading.Tasks;

namespace _0508_Book_Service
{
    [DataContract]
    public class Book
    {
        [DataMember]
        public int Id { get; set; }

        [DataMember]
        public string Title { get; set; }

        [DataMember]
        public string Author { get; set; }

        [DataMember]
        public int Price { get; set; }

        public Book()
        {

        }
        public Book(int id, string title, string author, int price)
        {
            Id = id;
            Title = title;
            Author = author;
            Price = price;
        }
    }
}

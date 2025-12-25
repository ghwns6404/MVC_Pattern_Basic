using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Security.Policy;
using System.Text;
using System.Threading.Tasks;

namespace _0507_PictureService
{
    public class Book
    {
        public int Isbn { get; set; }
        public string Title { get; set;}
        public string Author { get; set; }
        public string Publisher { get; set; }
        public int Price { get; set; }

        public Book(int isbn, string title, string author, string publisher, int price)
        {
            Isbn = isbn;
            Title = title;
            Author = author;
            Publisher = publisher;
            Price = price;
        }
        public Book()
        {

        }
        public Book(string title, string author, string publisher, int price)
        {
            Title = title;
            Author = author;
            Publisher = publisher;
            Price = price;
        }

        public override string ToString()
        {
            return $"{Isbn}#{Title}#{Author}#{Publisher}#{Price}";
        }

    }
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace book_DB
{
    public class Book
    {
        public int id;
        public string title;
        public string author;
        public int price;

        public Book()
        {

        }

        public Book(int _id, string _title, string _author, int _price)
        {
            id = _id;
            title = _title;
            author = _author;
            price = _price;
        }
    }
}

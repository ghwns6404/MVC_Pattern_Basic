////IPicture.cs

using _0507_PictureService;
using System.Collections.Generic;
using System.ServiceModel;
using System.Xml.Linq;

namespace _0507_PictureService
{
    internal class CBook : IBook
    {
        private  MyDB mydb = MyDB.Singleton;

        public bool C_Insert(int isbn, string title, string author, string publisher, int price)
        {
            return mydb.db_Insert(isbn, title, author, publisher, price);
        }

        public bool C_Update(int isbn, string title, string author, string publisher, int price)
        {
            return mydb.db_Update(isbn, title, author, publisher, price);
        }

        public string C_Select_Isbn(int isbn)
        {
            return mydb.db_Select_Isbn(isbn).ToString();
        }

        public bool C_Delete(int isbn)
        {
            return mydb.db_Delete(isbn);
        }

        public List<Book> C_Select_All()
        {
            return mydb.db_Select_All();
        }
    }
}

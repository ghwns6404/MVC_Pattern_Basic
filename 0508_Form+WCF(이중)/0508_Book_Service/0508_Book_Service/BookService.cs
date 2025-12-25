using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace _0508_Book_Service
{
    public class BookService : IBook
    {
        private MyDB mydb = MyDB.Singleton;

        public void Delete(int id)
        {
            mydb.db_Delete(id);
        }

        public bool Insert(int id, string title, string author, int price)
        {
            return mydb.db_Insert(id, title, author, price);   
        }

        public List<Book> Select(int id)
        {
            return mydb.db_Select_Id(id);
        }

        public List<Book> Select_All()
        {
            return mydb.db_Select_All();
        }

        public bool Update(int id, string title, string author, int price)
        {
            return mydb.db_Update(id, title, author, price);
        }
    }
}



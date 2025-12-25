using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;

namespace _0429_바인딩사용실습
{
    internal class BookControl
    {
        private Books books = null;

        public Books Books_
        {
            get { return books; }
            set { books = value; }
        }

        #region 싱글톤 패턴
        public static BookControl Singleton { get; } = null;
        static BookControl() { Singleton = new BookControl(); }
        private BookControl()
        {
        }
        #endregion

        #region insert delete update sort
        public void Insert_book(Book book)
        {
            books.Add(book);
        }

        public void Update_book(Book book,int idx)
        {
            Book findbook = books[idx];
            if (findbook == null)
                MessageBox.Show("BookControl에 UpdateBook~");

            findbook.Title = book.Title;
            findbook.Price = book.Price;
            findbook.Publisher = book.Publisher;
            findbook.Stock = book.Stock;
        }

        #endregion



    }
}

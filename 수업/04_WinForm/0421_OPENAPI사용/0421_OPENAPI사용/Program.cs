using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace _0421_OPENAPI사용
{
    internal class Program
    {
        static void Main(string[] args)
        {
            List<Book> books = NaverBook.Search("C언어");
            //int count = 0;
            foreach (Book book in books)
            {
                book.Println();

                //string fileName = string.Format($"image_{count++}.png");
                //NaverBook.DownloadRemoteImageFile(book.Image, fileName);                
            }
        }
    }
}

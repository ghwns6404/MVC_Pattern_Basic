//Book.cs
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

namespace _0421_OPENAPI사용
{
    internal class Book
    {
        #region 멤버필드 & 생성자
        public string Title { get; private set; }
        public string Link { get; private set; }
        public string Image { get; private set; }
        public string Author { get; private set; }
        public int Discount { get; private set; }
        public string Publisher { get; private set; }
        public string Pubdate { get; private set; }
        public string Isbn { get; private set; }
        public string Description { get; private set; }

        public Book(string title, string link, string image, string author, int discount, string publisher, string pubdate, string isbn, string description)
        {
            Title       = title;
            Link        = link;
            Image       = image;
            Author      = author;
            Discount    = discount;
            Publisher   = publisher;
            Pubdate     = pubdate;
            Isbn        = isbn;
            Description = description;
        }
        #endregion
        public static Book MakeBook(XmlNode xm)
        {
            string title        = xm.SelectSingleNode("title").InnerText;
            string link         = xm.SelectSingleNode("link").InnerText;
            string image        = xm.SelectSingleNode("image").InnerText;
            string author       = xm.SelectSingleNode("author").InnerText;
            int discount        = int.Parse(xm.SelectSingleNode("discount").InnerText);
            string publisher    = xm.SelectSingleNode("publisher").InnerText;
            string pubdate      = xm.SelectSingleNode("pubdate").InnerText;
            string isbn         = xm.SelectSingleNode("isbn").InnerText;
            string description  = xm.SelectSingleNode("description").InnerText;

            //파싱한 멤버필드(데이터)들을 호출시킨 Xml_Parsing으로 리턴시킴
            return new Book(title, link, image, author, discount, publisher, pubdate, isbn, description);
        }
    
        public void Println()
        {
            Console.WriteLine($"[Title]     {Title}");
            Console.WriteLine($"  [Link]      {Link}");
            Console.WriteLine($"  [Image]     {Image}");
            Console.WriteLine($"  [Author]    {Author}");
            Console.WriteLine($"  [Discount]  {Discount}원");
            Console.WriteLine($"  [Publisher] {Publisher}");
            Console.WriteLine($"  [Pubdate]   {Pubdate}");
            Console.WriteLine($"  [Isbn]      {Isbn}");
            Console.WriteLine($"  [Description]");
            Console.WriteLine($"      {Description}");
        }
    }
}

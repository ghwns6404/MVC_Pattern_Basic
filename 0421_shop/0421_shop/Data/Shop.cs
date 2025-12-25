using System;
using System.Collections.Generic;
using System.Data.SqlClient;
using System.Linq;
using System.Security.Policy;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

namespace WoosongBit41.Data
{
    internal class Shop
    {
        #region 멤버필드
        public long ProductId { get; private set; }
        public string Title { get; private set; }
        public string Link { get; private set; }
        public string Image { get; private set; }
        public int Lprice { get; private set; }
        public int Hprice { get; private set; }
        public string MallName { get; private set; }
        public int ProductType { get; private set; }
        public string Maker { get; private set; }
        public string Brand { get; private set; }
        public string Category1 { get; private set; }
        public string Category2 { get; private set; }
        public string Category3 { get; private set; }
        public string Category4 { get; private set; }

        #endregion

        #region 생성자's
        public Shop(long productId, string title, string link, string image, int lprice, int hprice, string mallName, int productType, string maker, string brand, string category1, string category2, string category3, string category4)
        {
            ProductId = productId;
            Title = title;
            Link = link;
            Image = image;
            Lprice = lprice;
            Hprice = hprice;
            MallName = mallName;
            ProductType = productType;
            Maker = maker;
            Brand = brand;
            Category1 = category1;
            Category2 = category2;
            Category3 = category3;
            Category4 = category4;
        }

        public Shop(string category1)
        {
            Category1 = category1;
        }
        #endregion


        #region 기능
        public static Shop MakeShop(XmlNode xm)
        {
            long productId = long.Parse(xm.SelectSingleNode("productId").InnerText);
            string title = xm.SelectSingleNode("title").InnerText;
            string link = xm.SelectSingleNode("link").InnerText;
            string image = xm.SelectSingleNode("image").InnerText;
            int lprice = int.Parse(xm.SelectSingleNode("lprice")?.InnerText ?? "-1");

            string hpriceStr = xm.SelectSingleNode("hprice").InnerText;
            int hprice = int.Parse(hpriceStr == string.Empty ? "-1" : hpriceStr);

            string mallName = xm.SelectSingleNode("mallName").InnerText;
            int productType = int.Parse(xm.SelectSingleNode("productType")?.InnerText ?? "-1");
            string maker = xm.SelectSingleNode("maker").InnerText;
            string brand = xm.SelectSingleNode("brand").InnerText;
            string category1 = xm.SelectSingleNode("category1").InnerText;
            string category2 = xm.SelectSingleNode("category2").InnerText;
            string category3 = xm.SelectSingleNode("category3").InnerText;
            string category4 = xm.SelectSingleNode("category4").InnerText;

            return new Shop(productId, title, link, image, lprice, hprice, mallName, productType, maker,
                brand, category1, category2, category3, category4);
        }

        public static Shop ParseShop(SqlDataReader r)
        {
            long productId = long.Parse(r["productId"].ToString());
            string title = r["Title"].ToString();
            string link = r["link"].ToString();
            string image = r["image"].ToString();
            int lprice = int.Parse(r["lprice"].ToString());
            int hprice = int.Parse(r["hprice"].ToString());
            string mallname = r["mallname"].ToString();
            int productType = int.Parse(r["productType"].ToString());
            string maker = r["maker"].ToString();
            string brand = r["brand"].ToString();
            string category1 = r["category1"].ToString();
            string category2 = r["category2"].ToString();
            string category3 = r["category3"].ToString();
            string category4 = r["category4"].ToString();

            return new Shop(productId, title, link, image, lprice, hprice, mallname,
                productType, maker, brand, category1, category2, category3, category4);
        }

        public void Println()
        {
            Console.WriteLine($"[ProductId] {ProductId}");
            Console.WriteLine($"[Title] {Title}");
            Console.WriteLine($"[Link] {Link}");
            Console.WriteLine($"[Lprice] {Lprice}");
            Console.WriteLine($"[Lprice] {Lprice}");
            Console.WriteLine($"[MallName] {MallName}");
            Console.WriteLine($"[ProductType] {ProductType}");
            Console.WriteLine($"[Maker] {Maker}");
            Console.WriteLine($"[Brand] {Brand}");
            Console.WriteLine($"[category1] {Category1}");
            Console.WriteLine($"[Category2] {Category2}");
            Console.WriteLine($"[Category3] {Category3}");
            Console.WriteLine($"[Category4] {Category4}");
        }
        public void Print2()
        {
            Console.WriteLine($"[category1] {Category1}");
        }
        #endregion
    }
}
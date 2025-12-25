using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net;
using System.Text;
using WoosongBit41.Data;
using System.Xml;
using System.Runtime.InteropServices;

namespace _0421_shop
{
    internal class MyAPI
    {
        private const string CLIENT_ID = "rWdkWza5e6IuXo0mHHvY";
        private const string CLIENT_SECRET = "RQHcniNIuw";

        private static XmlDocument doc = null;

        public static List<Shop> Search(string query)
        {
            string text = Sample(query);

            doc = new XmlDocument();
            doc.LoadXml(text);

            Xml_Print();
            return Xml_Parsing();
        }

        #region OpenAPI -> Parsing
        //1. OpenAPI 호출
        private static string Sample(string query, int display = 10)
        {
            //string url = "https://openapi.naver.com/v1/search/shop?query=" + query; // JSON 결과
            string url = "https://openapi.naver.com/v1/search/shop.xml?query=" + query + $"&display={display}";   
            HttpWebRequest request = (HttpWebRequest)WebRequest.Create(url);
            request.Headers.Add("X-Naver-Client-Id", CLIENT_ID); // 클라이언트 아이디
            request.Headers.Add("X-Naver-Client-Secret", CLIENT_SECRET);       // 클라이언트 시크릿
            HttpWebResponse response = (HttpWebResponse)request.GetResponse();
            string status = response.StatusCode.ToString();
            if (status == "OK")
            {
                Stream stream = response.GetResponseStream();
                StreamReader reader = new StreamReader(stream, Encoding.UTF8);
                string text = reader.ReadToEnd();
                return text;
            }
            else
            {
                throw new Exception(status);
            }
        }

        //2. XML 저장
        private static void Xml_Print()
        {
            doc.Save("shops.xml");
            //doc.Save(Console.Out);
        }

        //3. XML 데이터 파싱
        private static List<Shop> Xml_Parsing()
        {
            XmlNode node = doc.SelectSingleNode("rss");
            XmlNode n = node.SelectSingleNode("channel");

            List<Shop> shops = new List<Shop>();
            foreach (XmlNode item_node in n.SelectNodes("item"))
            {
                shops.Add(Shop.MakeShop(item_node));
            }
            return shops;
        }
        #endregion 

        public static bool DownloadRemoteImageFile(string uri, string fileName)
        {
            HttpWebRequest request = (HttpWebRequest)WebRequest.Create(uri);
            HttpWebResponse response = (HttpWebResponse)request.GetResponse();
            bool bImage = response.ContentType.StartsWith("image",
                StringComparison.OrdinalIgnoreCase);
            if ((response.StatusCode == HttpStatusCode.OK ||
                response.StatusCode == HttpStatusCode.Moved ||
                response.StatusCode == HttpStatusCode.Redirect) &&
                bImage)
            {
                using (Stream inputStream = response.GetResponseStream())
                using (Stream outputStream = File.OpenWrite(fileName))
                {
                    byte[] buffer = new byte[4096];
                    int bytesRead;
                    do
                    {
                        bytesRead = inputStream.Read(buffer, 0, buffer.Length);
                        outputStream.Write(buffer, 0, bytesRead);
                    } while (bytesRead != 0);
                }

                return true;
            }
            else
            {
                return false;
            }
        }
    }
}

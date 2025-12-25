//NaverBook.cs
using System;
using System.Collections.Generic;
using System.IO;
using System.Net;
using System.Text;
using System.Xml;

namespace _0421_OPENAPI사용
{
    internal class NaverBook
    {
        private const string CLIENT_ID      = "rWdkWza5e6IuXo0mHHvY";
        private const string CLIENT_SECRET  = "RQHcniNIuw";

        // 'doc'은 xml파일임
        private static XmlDocument doc = null;

        public static List<Book> Search(string query)
        {
            string text = Sample(query);

            doc         = new XmlDocument();
            doc.LoadXml(text);
            
            Xml_Print();
            return Xml_Parsing();
        }

        #region OpenAPI -> Parsing
        //1. OpenAPI 호출
        private static string Sample(string query)
        {
            //string url = "https://openapi.naver.com/v1/search/book?query=" + query; // JSON 결과

            ////http요청을 보내서 XML문자열로 응답을 받아옴
            string url = "https://openapi.naver.com/v1/search/book.xml?query=" + query + "&display=20";//30개
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
            doc.Save("books.xml"); //위Sample에서 응답받은 XML을 저장
            //doc.Save(Console.Out);    //디버깅할때 주석해제
        }

        //3. XML 데이터 파싱
        private static List<Book> Xml_Parsing()
        {
            XmlNode node = doc.SelectSingleNode("rss");
            XmlNode n    = node.SelectSingleNode("channel");

            List<Book> books = new List<Book>();
            foreach(XmlNode item_node in n.SelectNodes("item"))
            {
                books.Add( Book.MakeBook(item_node)); //MakeBook에서 리턴된(파싱된) 데이터들을 books리스트에 추가
            }
            return books;
        }
        #endregion 

        //public static bool DownloadRemoteImageFile(string uri, string fileName)
        //{
        //    HttpWebRequest request = (HttpWebRequest)WebRequest.Create(uri);
        //    HttpWebResponse response = (HttpWebResponse)request.GetResponse();
        //    bool bImage = response.ContentType.StartsWith("image", StringComparison.OrdinalIgnoreCase);
        //    if ((response.StatusCode == HttpStatusCode.OK ||
        //        response.StatusCode == HttpStatusCode.Moved ||
        //        response.StatusCode == HttpStatusCode.Redirect) &&
        //        bImage)
        //    {
        //        using (Stream inputStream = response.GetResponseStream())
        //        using (Stream outputStream = File.OpenWrite(fileName))
        //        {
        //            byte[] buffer = new byte[4096];
        //            int bytesRead;
        //            do
        //            {
        //                bytesRead = inputStream.Read(buffer, 0, buffer.Length);
        //                outputStream.Write(buffer, 0, bytesRead);
        //            } while (bytesRead != 0);
        //        }

        //        return true;
        //    }
        //    else
        //    {
        //        return false;
        //    }
        //}
    }
}

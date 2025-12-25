//mydb.cs

using System;
using System.Collections.Generic;
using System.Data.SqlClient;
using System.IO;
using System.Net.Sockets;
using System.ServiceModel.Channels;


namespace _0507_PictureService
{
    public class MyDB
    {
        List<Book> books = new List<Book>();
        #region 싱글톤
        public static MyDB Singleton { get; } = null;
        static MyDB() { Singleton = new MyDB(); }
        private MyDB() { }
        #endregion

        public bool db_Insert(int isbn, string title, string author, string publisher, int price)
        {
            string sql = string.Format($"insert into Book values({isbn}, '{title}', '{author}', '{publisher}', {price});");
            return ExSqlCommand(sql);
        }

        public bool db_Update(int isbn, string title, string author, string publisher, int price)
        {
            string sql = string.Format($"update Book set title ='{title}', author='{author}', publisher='{publisher}', price={price} where isbn={isbn};");
            return ExSqlCommand(sql);
        }

        public Book db_Select_Isbn(int isbn)
        {
            {
                SqlDataReader r = null;

                try
                {
                    Book b = new Book();
                    string sql = string.Format($"select title, author, publisher, price from Book where Isbn = {isbn};");
                    using (SqlCommand cmd = new SqlCommand(sql, scon))
                    {
                        r = cmd.ExecuteReader();
                        r.Read();

                        b.Title = r["Title"].ToString();
                        b.Author = r["Author"].ToString();
                        b.Publisher = r["Publisher"].ToString();
                        b.Price = int.Parse(r["Price"].ToString());

                    }
                    return b;
                }
                catch (Exception ex)
                {
                    Console.WriteLine(ex.Message);
                    return null;
                }
                finally
                {
                    r.Close();
                }
            }
        }

        public bool db_Delete(int isbn)
        {
            string sql = string.Format($"delete from book where isbn = {isbn};");
            return ExSqlCommand(sql);
        }

        public List<Book> db_Select_All()
        {
            string sql = string.Format($"Select * from book;");
            SqlDataReader r = null;
            using (SqlCommand cmd = new SqlCommand(sql, scon))
            {
                r = cmd.ExecuteReader();
                while (r.Read())
                {
                    int _isbn         = int.Parse(r["isbn"].ToString());
                    string _title     = r["title"].ToString();
                    string _author    = r["author"].ToString();
                    string _publisher = r["publisher"].ToString();
                    int _price        = int.Parse(r["price"].ToString());

                    books.Add(new Book(_isbn,_title,_author,_publisher,_price));
                }
                r.Close();
            }
            return books;
        }


        #region DB연결부
        private const string server_name = "BIT\\SQLEXPRESS";
        private const string db_name = "WB41";
        private const string sql_id = "aaa";
        private const string sql_pw = "1234";

        private SqlConnection scon = null;
        private SqlCommand scmd = null;
        #endregion

        #region Connect, Close
        public bool Connect()
        {
            try
            {
                string con = string.Format($"Data Source={server_name};Initial Catalog={db_name};User ID={sql_id};Password={sql_pw}");
                scon = new SqlConnection(con);
                scmd = scon.CreateCommand();
                scon.Open();
                return true;
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
                return false;
            }
        }
        public bool Close()
        {
            try
            {
                scon.Close();
                return true;
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
                return false;
            }
        }
        #endregion

        #region DB 명령 함수(ExecuteNonQuery:I,U,D),(ExcecuteScalar:S),(ExcecuteReader:S)
        public bool ExSqlCommand(string sql)
        {
            try
            {
                scmd.Connection = scon;
                scmd.CommandText = sql;

                if (scmd.ExecuteNonQuery() == 0)
                    return false;
                return true;
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
                return false;
            }
        }

        public object ExSqlScalarCommand(string sql)
        {
            try
            {
                scmd.Connection = scon;
                scmd.CommandText = sql;

                return scmd.ExecuteScalar();
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
                return null;
            }
        }

        #endregion


       


    }
}

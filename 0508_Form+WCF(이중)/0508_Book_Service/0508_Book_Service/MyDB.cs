using System;
using System.Collections.Generic;
using System.Data.SqlClient;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace _0508_Book_Service
{
    public class MyDB
    {
        #region 싱글톤
        public static MyDB Singleton { get; } = null;
        static MyDB() { Singleton = new MyDB(); }
        private MyDB() { }
        #endregion

        

        #region 기본기능 5형제
        public bool db_Insert(int id, string title, string author, int price)
        {
            string sql = string.Format($"insert into Book values({id}, '{title}', '{author}', {price});");
            return ExSqlCommand(sql);
        }

        public bool db_Update(int id, string title, string author, int price)
        {
            string sql = string.Format($"update Book set title ='{title}', author='{author}',  price={price} where id={id};");
            return ExSqlCommand(sql);
        }

        public List<Book> db_Select_Id(int id)
        {
            List<Book> books = new List<Book>();
            {
                SqlDataReader r = null;
                try
                {
                    Book b = new Book(); 
                    
                    string sql = string.Format($"select * from Book where id = {id};");
                    using (SqlCommand cmd = new SqlCommand(sql, scon))
                    {
                        r = cmd.ExecuteReader();
                        r.Read();

                        b.Title = r["Title"].ToString();
                        b.Author = r["Author"].ToString();
                        b.Price = int.Parse(r["Price"].ToString());

                        books.Add(b);
                    }
                    return books;
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

        public bool db_Delete(int id)
        {
            string sql = string.Format($"delete from book where id = {id};");
            return ExSqlCommand(sql);
        }

        public List<Book> db_Select_All()
        {
            List<Book> books = new List<Book>();

            string sql = string.Format($"Select * from book;");
            SqlDataReader r = null;
            using (SqlCommand cmd = new SqlCommand(sql, scon))
            {
                r = cmd.ExecuteReader();
                while (r.Read())
                {
                    int _id = int.Parse(r["id"].ToString());
                    string _title = r["title"].ToString();
                    string _author = r["author"].ToString();
                    int _price = int.Parse(r["price"].ToString());

                    books.Add(new Book(_id, _title, _author, _price));
                }
                r.Close();
            }
            return books;
        }


        #endregion

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

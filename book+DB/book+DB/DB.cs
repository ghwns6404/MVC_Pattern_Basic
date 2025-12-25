using System;
using System.Collections.Generic;
using System.Data.SqlClient;
using System.Diagnostics;
using System.Security.Policy;
using System.Xml.Linq;


namespace book_DB
{
    public class DB
    {
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

        public bool DB_Insert(int id, string title, string author, int price)
        {
            string sql = string.Format($"insert into book values({id}, '{title}', {author}, {price});");
            return ExSqlCommand(sql);
        }
        public string DB_Select(int id)
        {
            string sql = string.Format($"select * from book where id= {id};");
            scmd.CommandText = sql;
            using (SqlDataReader reader = scmd.ExecuteReader())
            {
                reader.Read();
                
                int _id = int.Parse(reader["id"].ToString());
                string _title = reader["title"].ToString();
                string _author = reader["author"].ToString();
                int _price = int.Parse(reader["price"].ToString());

                return string.Format($"{_id}%{_title}%{_author}%{_price}");
                
            }
        }
        public bool DB_Update(int id, string name, string author, int price)
        {
            string sql = string.Format($"update Book set title = '{name}', author = '{author}', price = {price} where id = {id};");
            return ExSqlCommand(sql);
        }
        public bool DB_Delete(int id)
        {
            string sql = string.Format($"delete from book where id = {id};");
            return ExSqlCommand(sql);
        }
        public List<Book> DB_PrintAll()
        {
            List<Book> book_list = new List<Book>();

            string sql = string.Format($"select * from book;");
            scmd.CommandText = sql;
            using(SqlDataReader reader = scmd.ExecuteReader())
            {

                while(reader.Read())
                {
                    int id = int.Parse(reader["id"].ToString());
                    string title = reader["title"].ToString();
                    string author= reader["author"].ToString();
                    int price = int.Parse(reader["price"].ToString());

                    book_list.Add(new Book(id, title, author, price));
                }
                return book_list;
            }
        }
    }
}

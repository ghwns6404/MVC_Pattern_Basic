//MyDB.cs
using System;
using System.Collections.Generic;
using System.Data.SqlClient;
using System.Diagnostics;
using System.Security.Cryptography;
using System.Xml.Linq;
using WoosongBit41.Data;
using WoosongBit41.Lib;
using static System.Net.Mime.MediaTypeNames;

namespace _0421_shop
{
    internal class MyDB
    {
        private const string server_name = "BIT\\SQLEXPRESS";
        private const string db_name = "WB41";
        private const string sql_id = "aaa";
        private const string sql_pw = "1234";

        private SqlConnection scon = null;
        private SqlCommand scmd = null;

        #region 연결, 종료
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

        #region 기능들
        public bool InsertShop(Shop shop) //1번
        {
            string sql = string.Format($"INSERT INTO Shop VALUES ('{shop.ProductId}', '{shop.Title}', '{shop.Link}', '{shop.Image}', '{shop.Lprice}', '{shop.Hprice}','{shop.MallName}','{shop.ProductType}',   '{shop.Maker}', '{shop.Brand}', '{shop.Category1}', '{shop.Category2}', '{shop.Category3}', '{shop.Category4}');");
            return ExSqlCommand(sql);

        }
        public List<Shop> PrintAll() //2번
        {
            List<Shop> list = new List<Shop>();
            string sql = string.Format($"select * from Shop;");
            using (SqlCommand cmd = new SqlCommand(sql, scon))
            {
                SqlDataReader r = cmd.ExecuteReader();  
                while (r.Read())
                {
                    list.Add(Shop.ParseShop(r));
                }
                r.Close();
            }
            return list;


        }
        public List<Shop> PrintCat() //3번
        {
            List<Shop> list = new List<Shop>();
            string sql = string.Format($"select distinct category1 from Shop;");
            using (SqlCommand cmd = new SqlCommand(sql, scon))
            {
                SqlDataReader r = cmd.ExecuteReader();
                while (r.Read())
                {  
                    string category1 = r["category1"].ToString();
                    list.Add(new Shop(category1));
                }
                r.Close();
            }
            return list;
        }
        public List<Shop> SelectCat(string category)
        {
            List<Shop> list = new List<Shop>();
            string sql = string.Format($"select * from Shop where category1 = '{category}';");
            using (SqlCommand cmd = new SqlCommand(sql, scon))
            {
                SqlDataReader r = cmd.ExecuteReader();
                while (r.Read())
                {

                    list.Add(Shop.ParseShop(r));
                }
                r.Close();
            }
            return list;
        }
        public List<Shop> SelectTitle(string _title)
        {
            List<Shop> list = new List<Shop>();
            string sql = string.Format($"SELECT * FROM shop WHERE title LIKE '%{_title}%';");
            using (SqlCommand cmd = new SqlCommand(sql, scon))
            {
                SqlDataReader r = cmd.ExecuteReader();
                while (r.Read())
                {

                    list.Add(Shop.ParseShop(r));
                }
                r.Close();
            }
            return list;
        }
        public List<Shop> Select_Max_Price()
        {
            List<Shop> list = new List<Shop>();
            string sql = string.Format($"select * from Shop where Lprice = (select MAX(lprice) from Shop);");
            using (SqlCommand cmd = new SqlCommand(sql, scon))
            {
                SqlDataReader r = cmd.ExecuteReader();
                while (r.Read())
                {
                    list.Add(Shop.ParseShop(r));
                }
                r.Close();
            }
            return list;
        }
        public List<Shop> Select_Min_Price()
        {
            List<Shop> list = new List<Shop>();
            string sql = string.Format($"select * from Shop where Lprice = (select MIN(lprice) from Shop);");
            using (SqlCommand cmd = new SqlCommand(sql, scon))
            {
                SqlDataReader r = cmd.ExecuteReader();
                while (r.Read())
                {
                    list.Add(Shop.ParseShop(r));
                }
                r.Close();
            }
            return list;
        }
        public bool DeleteAll()
        {
            string sql = string.Format($"delete from Shop;");
            return ExSqlCommand(sql);
        }


        #endregion

        #region DB 명령 함수(ExecuteNonQuery:I,U,D),(ExcecuteScalar:S),(ExcecuteReader:S)
        private bool ExSqlCommand(string sql)
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

        private object ExSqlScalarCommand(string sql)
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

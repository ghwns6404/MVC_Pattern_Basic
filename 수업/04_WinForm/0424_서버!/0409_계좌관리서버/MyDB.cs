using System;
using System.Collections.Generic;
using System.Data.SqlClient;
using System.Drawing;
using System.Xml.Linq;
using WoosongBit41.Data;
using WoosongBit41.Lib;

namespace _0409_계좌관리서버
{
    internal class MyDB
    {
        #region 연결, 접어놔
        private const string server_name = "BIT\\SQLEXPRESS";
        private const string db_name = "WB41";
        private const string sql_id = "aaa";
        private const string sql_pw = "1234";

        private SqlConnection scon = null;
        private SqlCommand scmd = null;
        #endregion

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

       

        public string Select_LogIn(string id, string pw)
        {
            string name = null;

            string sql = string.Format($"select name from chatting where id = '{id}' and pw = '{pw}';");
            
            SqlDataReader r = null;
            using (SqlCommand cmd = new SqlCommand(sql, scon))
            {
                r = cmd.ExecuteReader();
                r.Read();

                name = r["name"].ToString();

                r.Close();
            }
            return name;
        }
        public string Select_LogOut(string id)
        {
            string name = null;
            string sql = string.Format($"select name from chatting where id = '{id}';");

            SqlDataReader r = null;
            using (SqlCommand cmd = new SqlCommand(sql, scon))
            {
                r = cmd.ExecuteReader();
                r.Read();
         
                name = r["name"].ToString();
    
                r.Close();
            }
            return name;
        }

        public bool Update_LogIn(string id, string pw, bool b)
        {
            int log = 0;
            if (b == false)
                log = 0;
            else
                log = 1;

            string sql = string.Format($"update Chatting set isbool = {log} where id = '{id}' and pw = '{pw}';");
            return ExSqlCommand(sql);
        }
        public bool Update_LogInAndOut(string id,  bool b)
        {
            int log = -1;
            if (b == false)
                log = 0;
            else
                log = 1;
            string sql = string.Format($"update Chatting set isbool = {log} where id = '{id}';");
            return ExSqlCommand(sql);
        }
        public bool Insert_AddMemeber(string id, string pw, string name, Color color, DateTime date)
        {
            try
            {
                string sql = string.Format($"insert into Chatting values ('{id}', {0}, '{pw}','{name}','{color.Name}','{WbLib.Get_DateTime(date)}');");
                return ExSqlCommand(sql);
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
                return false;
            }
        }
       

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

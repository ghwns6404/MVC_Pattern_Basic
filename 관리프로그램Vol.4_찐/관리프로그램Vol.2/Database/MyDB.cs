using System;
using System.Collections.Generic;
using System.Data.SqlClient;

namespace 관리프로그램Vol._2
{
    public class MyDB
    {
        public List<Data> datas = new List<Data>();
        //Form1 form1 = new Form1();
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

        #region 기능's
        public bool InsertDB(string id, string pw, string name, string age, string phone)
        {
            string sql = string.Format($"insert into Users values('{id}', '{pw}', '{name}','{age}','{phone}');");
            return ExSqlCommand(sql);
        }
        public bool InsertDB_new(string id, string pw, string name, string age, string phone)
        {
            string sql = string.Format($"insert into logs values('{id}', '{pw}', '{name}','{age}','{phone}');");
            return ExSqlCommand(sql);
        }
        public List<Data> SelectDB()
        {
            List<Data> datas = new List<Data>();

            string sql = string.Format($"Select * from Users;");
            SqlDataReader r = null;
            using (SqlCommand cmd = new SqlCommand(sql, scon))
            {
                r = cmd.ExecuteReader();
                while (r.Read())
                {
                    string id    = r["Id"].ToString();
                    string pw    = r["pw"].ToString();
                    string name  = r["name"].ToString();
                    string age   = r["age"].ToString();
                    string phone = r["phone"].ToString();
                    
                    datas.Add(new Data(id, pw, name, age, phone));
                }
                r.Close();
            }
            return datas;
        }
        public List<Data> SelectDB_new()
        {
            List<Data> datas = new List<Data>();

            string sql = string.Format($"Select * from logs;");
            SqlDataReader r = null;
            using (SqlCommand cmd = new SqlCommand(sql, scon))
            {
                r = cmd.ExecuteReader();
                while (r.Read())
                {
                    string id = r["Id"].ToString();
                    string pw = r["pw"].ToString();
                    string name = r["name"].ToString();
                    string age = r["age"].ToString();
                    string phone = r["phone"].ToString();

                    datas.Add(new Data(id, pw, name, age, phone));
                }
                r.Close();
            }
            return datas;
        }
        public bool DeleteDB(string id)
        {
            string sql = string.Format($"delete from users where id = '{id}';");
            return ExSqlCommand(sql);
        }

        public bool ResetDB()
        {
            string sql = string.Format($"delete from Users;");
            return ExSqlCommand(sql);
        }

        public bool UpdateDB(string id, string pw,  string name, string age, string phone)
        {
            string sql = string.Format($"update Users set Pw ='{pw}',name='{name}',age='{age}',phone='{phone}'where id='{id}';");
            return ExSqlCommand(sql);
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

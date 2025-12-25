//MyDB.cs

using System;
using System.Collections.Generic;
using System.Data.SqlClient;


namespace Zoo_Service
{
    internal class MyDB
    {
        #region 싱글톤
        public static MyDB Singleton { get; } = null;
        static MyDB() { Singleton = new MyDB(); }
        private MyDB() { }
        #endregion

        #region 기본기능 5형제
        public bool db_Insert(int id, string name, string country, int age)
        {
            string sql = string.Format($"insert into Zoo values({id}, '{name}', '{country}', {age});");
            return ExSqlCommand(sql);
        }

        public bool db_Update(int id, string name, string country, int age)
        {
            string sql = string.Format($"update Zoo set name ='{name}', country='{country}',  Age={age} where id={id};");
            return ExSqlCommand(sql);
        }

        public Zoo db_Select_Id(int id)
        {
            Zoo zoos = new Zoo();
            {
                try
                {
                    string sql = string.Format($"select * from Zoo where id = {id};");
                    using (SqlCommand cmd = new SqlCommand(sql, scon))
                    {
                        SqlDataReader r = cmd.ExecuteReader();
                        r.Read();

                        string Name = r["Name"].ToString();
                        string Country = r["Country"].ToString();
                        int Age = int.Parse(r["Age"].ToString());

                        Zoo zoo = new Zoo(Name, Country,Age);

                        r.Close();

                        return zoo;
                    }
                }
                catch (Exception ex)
                {
                    Console.WriteLine(ex.Message);
                    return null;
                }
                
            }
        }

        public bool db_Delete(int id)
        {
            string sql = string.Format($"delete from Zoo where id = {id};");
            return ExSqlCommand(sql);
        }

        public List<Zoo> db_Select_All()
        {
            List<Zoo> zoos = new List<Zoo>();

            string sql = string.Format($"Select * from Zoo;");
            SqlDataReader r = null;
            using (SqlCommand cmd = new SqlCommand(sql, scon))
            {
                r = cmd.ExecuteReader();
                while (r.Read())
                {
                    int _id = int.Parse(r["id"].ToString());
                    string _name = r["name"].ToString();
                    string _country = r["country"].ToString();
                    int _age = int.Parse(r["age"].ToString());

                    zoos.Add(new Zoo(_id, _name, _country, _age));
                }
                r.Close();
            }
            return zoos;
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

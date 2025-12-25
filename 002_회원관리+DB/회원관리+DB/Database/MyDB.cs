//MyDB.cs
using System;
using System.Collections.Generic;
using System.Data.SqlClient;

namespace 회원관리_DB
{
    public class MyDB
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

        #region 기본 C R U D
        public bool DB_Insert(int id, string name, int age, string phone, string addr)
        {
            string sql = string.Format($"insert into Member1 values({id}, '{name}', {age}, '{phone}','{addr}');");
            return ExSqlCommand(sql);
        }
        public string DB_Select(int id)
        {
            string sql = string.Format($"select * from member1 where id = {id};");
            SqlDataReader r = null;
            try
            {
                using (SqlCommand cmd = new SqlCommand(sql, scon))
                {
                    r = cmd.ExecuteReader();
                    r.Read();

                    int _id = int.Parse(r["id"].ToString());
                    string _name = r["name"].ToString();
                    int _age = int.Parse(r["age"].ToString());
                    string _phone = r["phone"].ToString();
                    string _addr = r["addr"].ToString();

                    return string.Format($"{_id}%{_name}%{_age}%{_phone}%{_addr}");
                }
            }
            finally
            {
                r.Close();
            }
        }
        public bool DB_Update(int id, string name, int age, string phone, string addr)
        {
            string sql = string.Format($"update Member1 set name = '{name}', age = {age}, phone = '{phone}', addr = '{addr}' where id = {id};");
            return ExSqlCommand(sql);
        }
        public bool DB_Delete(int id)
        {
            string sql = string.Format($"delete from Member1 where id = {id};");
            return ExSqlCommand(sql);
        }
        #endregion

        #region 추가 C R U D
        public bool DB_DeleteAll()
        {
            string sql = string.Format($"delete from Member1;");
            return ExSqlCommand(sql);
        }
        public List<Member> DB_PrintAll()
        {
            List<Member> members = new List<Member>();

            string sql = string.Format($"select * from member1;");
            SqlDataReader r = null;
            try
            {
                using (SqlCommand cmd = new SqlCommand(sql, scon))
                {
                    r = cmd.ExecuteReader();
                    while (r.Read())
                    {
                        int _id = int.Parse(r["id"].ToString());
                        string _name = r["name"].ToString();
                        int _age = int.Parse(r["age"].ToString());
                        string _phone = r["phone"].ToString();
                        string _addr = r["addr"].ToString();

                        members.Add(new Member(_id, _name, _age, _phone, _addr));
                    }
                    return members;
                }
            }
            finally
            {
                r.Close();
            }
        }
        #endregion
    }
}

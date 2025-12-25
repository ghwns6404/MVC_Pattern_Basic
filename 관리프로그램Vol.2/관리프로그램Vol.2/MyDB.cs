using System;
using System.Collections.Generic;
using System.Data.SqlClient;
using static 관리프로그램Vol._2.Form1;


namespace 관리프로그램Vol._2
{
    internal class MyDB
    {
        public List<Data> datas = new List<Data>();
        

        private const string server_name = "BIT\\SQLEXPRESS";
        private const string db_name = "WB41";
        private const string sql_id = "aaa";
        private const string sql_pw = "1234";

        private SqlConnection scon = null;
        private SqlCommand scmd = null;

        public bool insert(string id, string pw, string name, string age, string phone)
        {
            string sql = string.Format($"insert into Users values({id}, {pw},{name},{age},{phone});");
            return ExSqlCommand(sql);
        }

        public void print()
        {
            string sql = string.Format($"select stu_name, sub_name, jumsu_num from jumsu , Student, Subject where (Jumsu.stu_id = Student.stu_id) and (Jumsu.sub_id = Subject.sub_id) and (Jumsu.stu_id = {stu_id} and Jumsu.sub_id = {sub_id});");
            SqlDataReader r = null;
            try
            {
                using (SqlCommand cmd = new SqlCommand(sql, scon))
                {
                    r = cmd.ExecuteReader();
                    r.Read();

                    string stu_name = r["stu_name"].ToString();
                    string sub_name = r["sub_name"].ToString();
                    string sub_name = r["sub_name"].ToString();
                    string sub_name = r["sub_name"].ToString();
                    string sub_name = r["sub_name"].ToString();
                    return string.Format($"{stu_name}%{sub_name}%{jumsu_num}");
                }
            }
        }


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

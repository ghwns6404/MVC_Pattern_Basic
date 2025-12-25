//MyDB.cs

using System;
using System.Collections.Generic;
using System.Data.SqlClient;
using System.Windows.Forms;
using Word_Quiz.Data;


namespace Word_Quiz.Database
{
    public class MyDB
    {

        #region 기능's
        public QuizData db_GetCorrect(string tableName)
        {
            string sql = $"SELECT TOP 1 * FROM {tableName} ORDER BY NEWID()";
            SqlCommand cmd = new SqlCommand(sql, scon);
            using (SqlDataReader r = cmd.ExecuteReader())
            {
                if (r.Read())
                {
                    {
                        QuizData data = new QuizData();
                        data.Word = r["Word"].ToString();
                        data.Meaning = r["Meaning"].ToString();
                        return data;
                    }
                }
            }
            return null;
        }

        public List<string> GetWrongChoices(string tableName, string correctWord)
        {
            string sql = $"SELECT TOP 3 Word FROM {tableName} WHERE Word != '{correctWord}' ORDER BY NEWID()";
            SqlCommand cmd = new SqlCommand(sql, scon);

            List<string> wrongs = new List<string>();
            using (SqlDataReader r = cmd.ExecuteReader())
            {
                while (r.Read())
                {
                    wrongs.Add(r["Word"].ToString());
                }
            }
            return wrongs;
        }
        #endregion

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

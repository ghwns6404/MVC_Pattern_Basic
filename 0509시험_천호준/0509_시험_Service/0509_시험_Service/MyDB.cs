using System;
using System.Collections.Generic;
using System.Data.SqlClient;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace _0509_시험_Service
{
    public class MyDB
    {
        #region 싱글톤
        public static MyDB Singleton { get; } = null;
        static MyDB() { Singleton = new MyDB(); }
        private MyDB() { }
        #endregion

        #region 기능's
        public bool db_Insert(int id, string name, int balacne) 
        {
            try
            {
                string sql = string.Format($"insert into Account values({id}, '{name}', {balacne});");
                return ExSqlCommand(sql);
            }
            catch
            {
                Console.WriteLine("Insert실패 !!");
                return false;
            }
            

        }

        public Account db_SelectAccount(int id)
        {
            Account account = new Account();
            try
            {
                string sql = string.Format($"select * from Account where id = {id};");
                using (SqlCommand cmd = new SqlCommand(sql, scon))
                {
                    SqlDataReader r = cmd.ExecuteReader();
                    r.Read();

                    int _id = int.Parse(r["Id"].ToString());
                    string _name = r["Name"].ToString();
                    int _balance = int.Parse(r["Balance"].ToString());

                    account = new Account(_id, _name, _balance);
                    r.Close();
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
                return null;
            }
            return account;
        }

        public bool db_Delete(int id)
        {
            string sql = string.Format($"delete from Account where id = {id};");
            return ExSqlCommand(sql);
        }

        public List<Account> db_SelectAllAccount()
        {
            List<Account> accounts = new List<Account>();

            string sql = string.Format($"Select * from Account;");

            SqlDataReader r = null;
            using (SqlCommand cmd = new SqlCommand(sql, scon))
            {
                r = cmd.ExecuteReader();
                while (r.Read())
                {
                    int _id = int.Parse(r["Id"].ToString());
                    string _name = r["Name"].ToString();
                    int _balance = int.Parse(r["Balance"].ToString());

                    accounts.Add(new Account(_id, _name, _balance));
                }
                r.Close();
            }
            return accounts;
        }

        public bool db_InputMoney(int id, int money)
        {
            string sql = string.Format($"update Account set Balance = Balance + {money} where Id = {id};");
            return ExSqlCommand(sql);
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

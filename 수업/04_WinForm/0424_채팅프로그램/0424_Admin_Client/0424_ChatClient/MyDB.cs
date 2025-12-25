/*
비연결형 데이터베이스

*/
using _0424_ChatClient;
using System;
using System.Collections.Generic;
using System.Data;
using System.Data.SqlClient;

namespace _0415_데이터베이스
{
    class MyDB
    {
        private const string server_name = "DESKTOP-K894V9T\\SQLEXPRESS";
        private const string db_name = "wb41";
        private const string sql_id = "aaa";
        private const string sql_pw = "1234";

        private SqlConnection scon = null;

        public MyDB()
        {
            string con = string.Format($"Data Source={server_name};Initial Catalog={db_name};User ID={sql_id};Password={sql_pw}");
            scon = new SqlConnection(con);

        }
        public DataTable GetMember_Table()
        {
            SqlDataAdapter sqlDataAdapter = new SqlDataAdapter();
            sqlDataAdapter.SelectCommand = MemberSelectAll();
            sqlDataAdapter.MissingSchemaAction = MissingSchemaAction.AddWithKey;

            DataTable dt = new DataTable("member");
            sqlDataAdapter.Fill(dt);
            return dt;
        }
        private SqlCommand MemberSelectAll()
        {
            string sql = "select * from Member";
            SqlCommand comm = new SqlCommand(sql, scon);
            return comm;
        }
    }
}

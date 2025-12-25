using System.Data.SqlClient;
using System.Data;


namespace _0424_ChatClient
{
    internal class MyDB
    {

        private const string server_name = "BIT\\SQLEXPRESS";
        private const string db_name = "WB41";
        private const string sql_id = "aaa";
        private const string sql_pw = "1234";

        private SqlConnection scon = null;
        
        public MyDB()
        {
            string con = string.Format($"Data Source={server_name};Initial Catalog={db_name};User ID={sql_id};Password={sql_pw}");
            scon = new SqlConnection(con);
        }
        public DataTable GetMemberTable()
        {
            SqlDataAdapter sqlDataAdapter = new SqlDataAdapter();
            sqlDataAdapter.SelectCommand = MemberSelectAll();
            sqlDataAdapter.MissingSchemaAction = MissingSchemaAction.AddWithKey;

            DataTable data = new DataTable();
            sqlDataAdapter.Fill(>?????, "member");
            return data;
        }
        private SqlCommand MemberSelectAll()
        {
            string sql = "select * from s_custom";
            SqlCommand comm = new SqlCommand(sql, scon);
            return comm;
        }
        //Select 명령 등록
        
       
    }
}


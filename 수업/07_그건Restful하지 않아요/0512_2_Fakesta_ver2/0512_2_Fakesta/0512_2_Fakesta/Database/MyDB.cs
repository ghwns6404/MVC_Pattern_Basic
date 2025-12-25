//MyDB.cs
using _0512_2_Fakesta.Data;
using System;
using System.Collections.Generic;
using System.Data.SqlClient;
using System.Text;
using System.Xml.Linq;

namespace _0512_2_Fakesta
{
    internal class MyDB
    {
        Post post = new Post();
        

        DateTime created_at = DateTime.Now;
        DateTime updated_at = DateTime.Now;

        #region 전체출력 (게시물 / 사용자)
        public List<Post> db_Select_Post_All()
        {
            List<Post> posts = new List<Post>();

            string sql = string.Format($"select * from Post");
            SqlDataReader r = null;
            using (SqlCommand cmd = new SqlCommand(sql, scon))
            {
                r = cmd.ExecuteReader();
                while (r.Read())
                {
                    int id = int.Parse(r["Id"].ToString());
                    int mem_id = int.Parse(r["mem_id"].ToString());
                    string name = r["name"].ToString();
                    string content = r["content"].ToString();
                    DateTime created_at = DateTime.Parse(r["created_at"].ToString());
                    DateTime updated_at = DateTime.Parse(r["updated_at"].ToString());

                    posts.Add(new Post(id, mem_id, name, content, created_at, updated_at));
                }
                r.Close();
            }
            return posts;
        }
        public List<Member> db_Select_Member_All()
        {
            List<Member> members = new List<Member>();

            string sql = string.Format($"select * from Member");
            SqlDataReader r = null;
            using (SqlCommand cmd = new SqlCommand(sql, scon))
            {
                r = cmd.ExecuteReader();
                while (r.Read())
                {
                    int member_id = int.Parse(r["id"].ToString());
                    string name = r["name"].ToString();
                    string email = r["email"].ToString();
                    DateTime created_at = DateTime.Parse(r["created_at"].ToString());
                    DateTime updated_at = DateTime.Parse(r["updated_at"].ToString());

                    members.Add(new Member(member_id, name, email, created_at.ToString(), updated_at.ToString()));
                }
                r.Close();
            }
            return members;
        }
        #endregion

        #region 단일 조회 (게시물 / 사용자(보류))
        public Post db_Select_Post_title(int id)//ㅇㅋ
        {
            try
            {
                string sql = string.Format($"select * from post where id = {id};");
                using (SqlCommand cmd = new SqlCommand(sql, scon))
                {
                    SqlDataReader r = cmd.ExecuteReader();
                    r.Read();

                    id = int.Parse(r["id"].ToString());
                    int memid = int.Parse(r["mem_id"].ToString());
                    string name = r["name"].ToString();
                    string content = r["content"].ToString();
                    DateTime created_at = DateTime.Parse(r["created_at"].ToString());
                    DateTime updated_at = DateTime.Parse(r["updated_at"].ToString());

                    Post post = new Post(id, memid, name, content, created_at, updated_at);

                    r.Close();

                    return post;
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
                return null;
            }
        }
        public Member db_Select_Member_Id(int id)//ㅇㅋ
        {
            try
            {
                string sql = string.Format($"select * from Member where id = {id};");
                using (SqlCommand cmd = new SqlCommand(sql, scon))
                {
                    SqlDataReader r = cmd.ExecuteReader();
                    r.Read();

                    string name = r["name"].ToString();
                    string email = r["email"].ToString();
                     
                    Member member = new Member(name, email);

                    r.Close();

                    return member;
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
                return null;
            }
        }
        #endregion

        #region 추가 (게시물 / 사용자)
        public bool db_Insert_Post(string token, string name, string content)
        {
            try
            {
                SqlCommand cmd1 = new SqlCommand("SELECT mem_id FROM session WHERE token = @Token;", scon);
                cmd1.Parameters.AddWithValue("@Token", token);

                Console.WriteLine("넘겨받은 token: " + token);

                int mem_id = -1;
                using (SqlDataReader reader = cmd1.ExecuteReader())
                {
                    if (!reader.Read()) return false;
                    mem_id = int.Parse(reader["mem_id"].ToString());
                }

                SqlCommand cmd2 = new SqlCommand(
                    "INSERT INTO Post (mem_id, name, content, created_at, updated_at) VALUES (@mem_id, @name, @content, @created_at, @updated_at)",
                    scon
                );
                cmd2.Parameters.AddWithValue("@mem_id", mem_id);
                cmd2.Parameters.AddWithValue("@name", name);
                cmd2.Parameters.AddWithValue("@content", content);
                cmd2.Parameters.AddWithValue("@created_at", Post.Get_DateTime(created_at));
                cmd2.Parameters.AddWithValue("@updated_at", Post.Get_DateTime(updated_at));

                return cmd2.ExecuteNonQuery() > 0;
            }
            catch (Exception ex)
            {
                Console.WriteLine("에러: " + ex.Message);
                return false;
            }
        }
        public bool db_Insert_Member(string name, string email, string pw, DateTime created_at, DateTime updated_at)//ㅇㅋ
        {
            string sql = string.Format($"insert into Member values('{name}','{email}', '{pw}', '{Post.Get_DateTime(created_at)}', '{Post.Get_DateTime(updated_at)}');");
            return ExSqlCommand(sql);
        }
        #endregion
            
        #region 로그인
        private Random _random = new Random();
        public DateTime create_at = DateTime.Now;
        public DateTime expires_at = DateTime.Now.AddMinutes(30); // 30분 후 만료  

        /// <summary>
        /// 로그인 요청시 중복 검사 후 세션테이블에 (랜덤)토큰을, 생성일, 만료일 추가합니다.
        /// </summary>
        public string db_Login(string email, string pw, DateTime created_at, DateTime expires_at)
        {
            //member_id를 찾음 즉 member_id 가  temp_mem_id에 담겨있음 !!!!
            int temp_mem_id =  db_Login_Check(email, pw).Id;

            //토큰생성
            string token = db_RandomString(30, true);
            string sql = string.Format($"insert into Session values({temp_mem_id}, '{token}', '{Post.Get_DateTime(created_at)}', '{Post.Get_DateTime(expires_at)}');");

            if(ExSqlCommand(sql) == false)
                throw new Exception("세션 추가 실패");
            return token;
        }
        public string db_RandomString(int size, bool lowerCase = false)//랜덤문자열 생성
        {
            var builder = new StringBuilder(size);

            // 알파벳 대문자(A-Z) 또는 소문자(a-z)를 생성하기 위한 기준 문자 설정
            char offset = lowerCase ? 'a' : 'A';
            const int lettersOffset = 26; // 알파벳 개수

            for (var i = 0; i < size; i++)
            {
                char randtoken = (char)_random.Next(offset, offset + lettersOffset);
                builder.Append(randtoken);
            }

            return lowerCase ? builder.ToString().ToLower() : builder.ToString();
        }
        public Member db_Login_Check(string email, string pw)
        {
            string sql = string.Format($"select id from Member where email = '{email}' and pw = '{pw}';");
            using (SqlCommand cmd = new SqlCommand(sql, scon))
            {
                SqlDataReader r = cmd.ExecuteReader();
                
                if(r.Read()== false)
                {
                    r.Close();
                    return null;
                }
                int member_id = int.Parse(r["id"].ToString());

                Member member = new Member(member_id);

                r.Close();

                return member;
            }
        }
        #endregion

        #region 게시물 삭제
        public bool db_Delete_Post(int id)
        {
            string sql = string.Format($"delete from post where id = {id};");
            return ExSqlCommand(sql);
        }

        #endregion











        #region 싱글톤
        public static MyDB Singleton { get; } = null;
        static MyDB() { Singleton = new MyDB(); }
        private MyDB() { }
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

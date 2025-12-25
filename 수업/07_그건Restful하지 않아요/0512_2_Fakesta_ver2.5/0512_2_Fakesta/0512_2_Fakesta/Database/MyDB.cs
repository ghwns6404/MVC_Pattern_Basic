//MyDB.cs
using _0512_2_Fakesta.Data;
using Microsoft.Bot.Builder.Integration.AspNet.Core;
using System;
using System.Collections.Generic;
using System.Data;
using System.Data.SqlClient;
using System.IO;
using System.ServiceModel.Web;
using System.Text;
using System.Web.UI.WebControls;
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
                    string created_at = DateTime.Parse(r["created_at"].ToString()).ToString("O");
                    string updated_at = DateTime.Parse(r["updated_at"].ToString()).ToString("O");
                    

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
                    string created_at = DateTime.Parse(r["created_at"].ToString()).ToString("O");
                    string updated_at = DateTime.Parse(r["updated_at"].ToString()).ToString("O");

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
                    string created_at = DateTime.Parse(r["created_at"].ToString()).ToString("O");
                    string updated_at = DateTime.Parse(r["updated_at"].ToString()).ToString("O");

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
        public bool db_Insert_Post(string token, string title, string content, List<(string name, byte[] content)> files)//게시물 추가 (일단ㅇㅋ
        {
            try
            {
                // 1. 토큰으로 mem_id 가져오기
                SqlCommand cmd1 = new SqlCommand("SELECT mem_id FROM Session WHERE token = @token;", scon);
                cmd1.Parameters.AddWithValue("@token", token);

                int mem_id = -1;
                using (SqlDataReader reader = cmd1.ExecuteReader())
                {
                    if (!reader.Read()) return false;
                    mem_id = int.Parse(reader["mem_id"].ToString());
                }

                // 2. Post에 글 추가
                SqlCommand cmd2 = new SqlCommand(@"
            INSERT INTO Post (mem_id, title, content, created_at, updated_at) 
            OUTPUT INSERTED.id 
            VALUES (@mem_id, @title, @content, @created_at, @updated_at);", scon);
                cmd2.Parameters.AddWithValue("@mem_id", mem_id);
                cmd2.Parameters.AddWithValue("@title", title);
                cmd2.Parameters.AddWithValue("@content", content);
                cmd2.Parameters.AddWithValue("@created_at", DateTime.Now);
                cmd2.Parameters.AddWithValue("@updated_at", DateTime.Now);

                int post_id = (int)cmd2.ExecuteScalar();

                // 3. 파일 저장 + PostFile에 insert
                foreach (var file in files)
                {
                    string fileName = file.name;
                    byte[] fileContent = file.content;
                    string filePath = @"aaa\" + fileName;

                    // 파일 저장
                    using (FileStream fs = new FileStream(filePath, FileMode.Create, FileAccess.Write))
                    using (BinaryWriter bw = new BinaryWriter(fs))
                    {
                        bw.Write(fileContent);
                    }

                    // DB에 파일 정보 저장
                    SqlCommand cmd3 = new SqlCommand(@"
                INSERT INTO PostFile (post_id, name, path, created_at)
                VALUES (@post_id, @name, @path, @created_at);", scon);
                    cmd3.Parameters.AddWithValue("@post_id", post_id);
                    cmd3.Parameters.AddWithValue("@name", fileName);
                    cmd3.Parameters.AddWithValue("@path", filePath);
                    cmd3.Parameters.AddWithValue("@created_at", DateTime.Now);

                    if (cmd3.ExecuteNonQuery() == 0)
                        return false;
                } 
                return true;
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
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

        #region 사용자 삭제
        public bool db_Delete_Member(string token)
        {
            //string sql = string.Format($"delete from members where token = '{token}';");
            //return ExSqlCommand(sql);

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

              

                SqlCommand cmd2 = new SqlCommand("Delete from Member WHERE id = @id", scon);
                cmd2.Parameters.AddWithValue("@id", mem_id);

               

                return cmd2.ExecuteNonQuery() > 0;
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
                return false;
            }
        }
        #endregion

        #region 로그인된 회원 조회
        public Member db_Select_Member_Me(string token)
        {
            try
            {
                string sql = string.Format($"select mem_id from session where token = '{token}';");
                using (SqlCommand cmd = new SqlCommand(sql, scon))
                {
                    SqlDataReader r = cmd.ExecuteReader();
                    r.Read();

                    int mem_id = int.Parse(r["mem_id"].ToString());

                    Member member = new Member(mem_id);

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

        #region 로그인된 회원 비밀번호 변경
        public bool db_Update_Member_Pw(string token, string pw, string newPw)
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

                SqlCommand cmd2 = new SqlCommand("UPDATE Member SET pw = @pw WHERE id = @id", scon);
                cmd2.Parameters.AddWithValue("@id", mem_id);
                cmd2.Parameters.AddWithValue("@pw", newPw);

                return cmd2.ExecuteNonQuery() > 0;
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
                return false;
            }
        }
        #endregion

        #region 현재 로그인된 회원의 세션 조회
        public Session db_Select_Session_Whoami(string token)
        {
            Session sessions = new Session();
            try
            {
                string sql = string.Format($"select * from session where token = '{token}';");

                using (SqlCommand cmd = new SqlCommand(sql, scon))
                {
                    SqlDataReader r = cmd.ExecuteReader();
                    r.Read();
                    int mem_id = int.Parse(r["mem_id"].ToString());
                    string created_at = DateTime.Parse(r["created_at"].ToString()).ToString("O");
                    string expires_at = DateTime.Parse(r["expires_at"].ToString()).ToString("O");

                    sessions = new Session(mem_id, created_at, expires_at);

                    r.Close();

                    return sessions;
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
                return null;
            }
        }

        #endregion

        #region 현재 로그인된 세션 제외한 모든 세션 조회
        public List<Session> db_Select_Session_Me(string token)
        {
            List<Session> sessions = new List<Session>();
            string sql = string.Format($"select * from session where token != '{token}';");
            SqlDataReader r = null;
            using (SqlCommand cmd = new SqlCommand(sql, scon))
            {
                r = cmd.ExecuteReader();
                while (r.Read())
                {
                    int mem_id = int.Parse(r["mem_id"].ToString());
                    string created_at = DateTime.Parse(r["created_at"].ToString()).ToString("O");
                    string expires_at = DateTime.Parse(r["expires_at"].ToString()).ToString("O");

                    sessions.Add(new Session(mem_id, created_at, expires_at));
                }
                r.Close();
            }
            return sessions;
        }
        #endregion

        #region 세션 로그아웃
        public bool db_Delete_Session(int id)
        {
            string sql = string.Format($"delete from session where id = {id};");
            return ExSqlCommand(sql);
        }
        #endregion

        #region 게시물 파일 아이디로 조회
        public Stream db_Select_Post_FileId(int id)
        {
            try
            {
                string sql = $"SELECT path FROM postfile WHERE id = @id;";
                using (SqlCommand cmd = new SqlCommand(sql, scon))
                {
                    cmd.Parameters.AddWithValue("@id", id);
                    SqlDataReader r = cmd.ExecuteReader();

                    if (!r.Read())
                        return null;

                    string path = r["path"].ToString(); // 경로 문자열
                    r.Close();

                    if (!File.Exists(path))
                        return null;

                    byte[] imgBytes = File.ReadAllBytes(path);

                    // 확장자 기반 Content-Type 설정 (C# 7.3 호환)
                    string extension = Path.GetExtension(path).ToLower();
                    string contentType = "application/octet-stream";

                    switch (extension)
                    {
                        case ".jpg":
                        case ".jpeg":
                            contentType = "image/jpeg";
                            break;
                        case ".png":
                            contentType = "image/png";
                            break;
                        case ".gif":
                            contentType = "image/gif";
                            break;
                        case ".bmp":
                            contentType = "image/bmp";
                            break;
                    }

                    WebOperationContext.Current.OutgoingResponse.ContentType = contentType;

                    return new MemoryStream(imgBytes);
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
                return null;
            }
        }





        #endregion

        //---------------------------------------------------------------------

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

// MyDB.cs

using _0513_2_Fakesta.Data;
using System.Collections.Generic;
using System.Data.SqlClient;
using System;
using System.IO;
using _0513_2_Fakesta.DBData;
using _0513_2_Fakesta.Error;
using System.Security.Cryptography;

namespace _0513_2_Fakesta.Database
{
    internal class MyDB : IDisposable
    {
        #region 싱글턴

        public static MyDB Instance;
        static MyDB()
        {
            Instance = new MyDB();
        }
        private MyDB() { }

        #endregion

        #region 멤버 필드

        private SqlConnection _scon;

        private SqlCommand _scmd;

        #endregion

        #region 연결 관련 메서드

        public bool Init(string serverName, string dbName, string uid, string pwd)
        {
            string conStr = $"Data Source={serverName}; Initial Catalog={dbName}; User ID={uid}; Password={pwd}";

            _scon = new SqlConnection(conStr);

            try
            {
                _scon.Open();

                _scmd = _scon.CreateCommand();

                return true;
            }
            catch (Exception ex)
            {
                Console.WriteLine("데이터베이스 연결을 실패했습니다. {0}", ex.Message);
                return false;
            }
        }

        public bool Close()
        {
            try
            {
                _scon.Close();

                return true;
            }
            catch (Exception ex)
            {
                Console.WriteLine("데이터베이스 연결 해제를 실패했습니다. {0}", ex.Message);
                return false;
            }
        }

        public void Dispose()
        {
            Close();
        }

        #endregion

        #region Member 관련 메서드

        public int InsertMember(string email, string pw, string name)
        {
            DateTime now = DateTime.Now;

            string sql = $"INSERT INTO Member VALUES('{email}', '{pw}', '{name}', '{now:O}', '{now:O}'); SELECT SCOPE_IDENTITY();";

            object id = ExecScalar(sql);
            if (id == null)
                throw new ApplicationException("회원을 추가할 수 없습니다.");

            return int.Parse(id.ToString());
        }

        public Member SelectMemberById(int id)
        {
            string sql = $"SELECT Member.id AS m_id, Member.email AS m_email, Member.name AS m_name, Member.created_at AS m_created_at, Member.updated_at AS m_updated_at FROM Member WHERE id = {id};";
            _scmd.CommandText = sql;

            using (SqlDataReader reader = _scmd.ExecuteReader())
            {
                if (!reader.Read())
                    throw new FakestaMemberNotFoundException($"회원({id})를 찾을 수 없습니다.");

                return ReadMember(reader);
            }
        }

        public List<Member> SelectAllMember()
        {
            string sql = "SELECT Member.id AS m_id, Member.email AS m_email, Member.name AS m_name, Member.created_at AS m_created_at, Member.updated_at AS m_updated_at FROM Member;";
            _scmd.CommandText = sql;

            var members = new List<Member>();
            using (SqlDataReader reader = _scmd.ExecuteReader())
            {
                while (reader.Read())
                    members.Add(ReadMember(reader));
            }
            return members;
        }

        public void UpdateMemberPw(int id, string pw)
        {
            string sql = $"UPDATE Member SET pw = '{pw}' WHERE id = {id};";

            if (ExecNonQuery(sql) < 1)
                throw new FakestaMemberNotFoundException($"회원({id})를 찾을 수 없습니다.");
        }

        public void DeleteMember(int id)
        {
            string sql = $"DELETE FROM Member WHERE id = {id};";

            if (ExecNonQuery(sql) < 1)
                throw new FakestaMemberNotFoundException($"회원({id})를 찾을 수 없습니다.");
        }

        public Member CheckMemberPw(int id, string pw)
        {
            string sql = $"SELECT Member.id AS m_id, Member.email AS m_email, Member.name AS m_name, Member.created_at AS m_created_at, Member.updated_at AS m_updated_at FROM Member WHERE id = {id} AND pw = '{pw}';";
            _scmd.CommandText = sql;

            using (SqlDataReader reader = _scmd.ExecuteReader())
            {
                if (!reader.Read())
                    throw new FakestaAuthException("인증 정보가 유효하지 않습니다.");

                return ReadMember(reader);
            }
        }

        public Member CheckMemberPw(string email, string pw)
        {
            string sql = $"SELECT Member.id AS m_id, Member.email AS m_email, Member.name AS m_name, Member.created_at AS m_created_at, Member.updated_at AS m_updated_at FROM Member WHERE email = '{email}' AND pw = '{pw}';";
            _scmd.CommandText = sql;

            using (SqlDataReader reader = _scmd.ExecuteReader())
            {
                if (!reader.Read())
                    throw new FakestaAuthException("인증 정보가 유효하지 않습니다.");

                return ReadMember(reader);
            }
        }

        #endregion

        #region Session 관련 메서드

        public (int, string) InsertSession(int memId)
        {
            string token = GenerateRandomSessionToken();
            DateTime now = DateTime.Now;
            DateTime expiresAt = now.AddDays(30);

            string sql = $"INSERT INTO Session VALUES({memId}, '{token}', '{now:O}', '{expiresAt:O}'); SELECT SCOPE_IDENTITY();";

            object id = ExecScalar(sql);
            if (id == null)
                throw new ApplicationException("세션을 추가할 수 없습니다.");

            return (int.Parse(id.ToString()), token);
        }

        public DSession SelectSessionByToken(string token)
        {
            string sql = $"SELECT Session.id AS s_id, Session.mem_id AS s_mem_id, Session.token AS s_token, Session.created_at AS s_created_at, Session.expires_at AS s_expires_at, Member.id AS m_id, Member.email AS m_email, Member.name AS m_name, Member.created_at AS m_created_at, Member.updated_at AS m_updated_at FROM Session, Member WHERE Session.mem_id = Member.id AND token = '{token}';";
            _scmd.CommandText = sql;

            using (SqlDataReader reader = _scmd.ExecuteReader())
            {
                if (!reader.Read())
                    throw new FakestaSessionNotFoundException($"세션(token: {token})을 찾을 수 없습니다.");

                return ReadSession(reader);
            }
        }

        public List<DSession> SelectAllMemberSession(int memId)
        {
            string sql = $"SELECT Session.id AS s_id, Session.mem_id AS s_mem_id, Session.token AS s_token, Session.created_at AS s_created_at, Session.expires_at AS s_expires_at, Member.id AS m_id, Member.email AS m_email, Member.name AS m_name, Member.created_at AS m_created_at, Member.updated_at AS m_updated_at FROM Session, Member WHERE Session.mem_id = Member.id AND Session.mem_id = {memId};";
            _scmd.CommandText = sql;

            var sessions = new List<DSession>();
            using (SqlDataReader reader = _scmd.ExecuteReader())
            {
                while (reader.Read())
                    sessions.Add(ReadSession(reader));
            }
            return sessions;
        }

        public void DeleteSession(int id, int memId)
        {
            string sql = $"DELETE FROM Session WHERE id = {id} AND mem_id = {memId};";

            if (ExecNonQuery(sql) < 1)
                throw new FakestaSessionNotFoundException($"세션({id})을 찾을 수 없습니다.");
        }

        #endregion

        #region Post 관련 메서드

        public int InsertPost(int memId, string title, string content)
        {
            DateTime now = DateTime.Now;

            string sql = $"INSERT INTO Post VALUES({memId}, '{title}', '{content}', '{now:O}', '{now:O}'); SELECT SCOPE_IDENTITY();";

            object id = ExecScalar(sql);
            if (id == null)
                throw new ApplicationException("게시물을 추가할 수 없습니다.");

            return int.Parse(id.ToString());
        }

        public List<DPost> SelectAllPost()
        {
            string sql = "SELECT Post.id AS p_id, Post.mem_id AS p_mem_id, Post.title AS p_title, Post.content AS p_content, Post.created_at AS p_created_at, Post.updated_at AS p_updated_at, Member.id AS m_id, Member.email AS m_email, Member.name AS m_name, Member.created_at AS m_created_at, Member.updated_at AS m_updated_at FROM Post, Member WHERE Post.mem_id = Member.id;";
            _scmd.CommandText = sql;

            var posts = new List<DPost>();
            using (SqlDataReader reader = _scmd.ExecuteReader())
            {
                while (reader.Read())
                    posts.Add(ReadPost(reader));
            }

            foreach (DPost post in posts)
                post.Files = SelectAllPostFileByPostId(post.Id);

            return posts;
        }

        public DPost SelectPostById(int id)
        {
            string sql = $"SELECT Post.id AS p_id, Post.mem_id AS p_mem_id, Post.title AS p_title, Post.content AS p_content, Post.created_at AS p_created_at, Post.updated_at AS p_updated_at, Member.id AS m_id, Member.email AS m_email, Member.name AS m_name, Member.created_at AS m_created_at, Member.updated_at AS m_updated_at FROM Post, Member WHERE Post.mem_id = Member.id AND Post.id = {id};";
            _scmd.CommandText = sql;

            DPost post;
            using (SqlDataReader reader = _scmd.ExecuteReader())
            {
                if (!reader.Read())
                    throw new FakestaPostNotFoundException($"게시물({id})을 찾을 수 없습니다.");

                post = ReadPost(reader);
            }

            post.Files = SelectAllPostFileByPostId(post.Id);

            return post;
        }

        public void DeletePost(int id, int memId)
        {
            string sql = $"DELETE FROM Post WHERE id = {id} AND mem_id = {memId};";

            if (ExecNonQuery(sql) < 1)
                throw new FakestaSessionNotFoundException($"게시물({id})을 찾을 수 없습니다.");
        }

        #endregion

        #region PostFile 관련 메서드

        public int InsertPostFile(int postId, string name, string filePath, Stream stream)
        {
            DateTime now = DateTime.Now;

            string sql = $"INSERT INTO PostFile VALUES({postId}, '{name}', '{now:O}'); SELECT SCOPE_IDENTITY();";

            object oid = ExecScalar(sql);
            if (oid == null)
                throw new ApplicationException("게시물 파일을 추가할 수 없습니다.");

            int id = int.Parse(oid.ToString());

            string path = $"{filePath}/post-files/{id}.png";
            using (FileStream fs = new FileStream(path, FileMode.Create))
            {
                stream.CopyTo(fs);
            }

            return id;
        }

        public List<DPostFile> SelectAllPostFileByPostId(int id)
        {
            using (SqlCommand scmd = _scon.CreateCommand())
            {
                string sql = $"SELECT PostFile.id AS pf_id, PostFile.post_id AS pf_post_id, PostFile.name AS pf_name, PostFile.created_at AS pf_created_at FROM PostFile WHERE PostFile.post_id = {id};";
                scmd.CommandText = sql;

                var postFiles = new List<DPostFile>();
                using (SqlDataReader reader = scmd.ExecuteReader())
                {
                    while (reader.Read())
                        postFiles.Add(ReadPostFile(reader));
                }
                return postFiles;
            }
        }

        public DPostFile SelectPostFileById(int id)
        {
            string sql = $"SELECT PostFile.id AS pf_id, PostFile.post_id AS pf_post_id, PostFile.name AS pf_name, PostFile.created_at AS pf_created_at FROM PostFile WHERE id = {id};";
            _scmd.CommandText = sql;

            using (SqlDataReader reader = _scmd.ExecuteReader())
            {
                if (!reader.Read())
                    throw new FakestaPostFileNotFoundException($"게시물 파일({id})을 찾을 수 없습니다.");

                return ReadPostFile(reader);
            }
        }

        #endregion

        #region 내부적인 메서드

        private int ExecNonQuery(string sql)
        {
            _scmd.CommandText = sql;

            return _scmd.ExecuteNonQuery();
        }

        private object ExecScalar(string sql)
        {
            _scmd.CommandText = sql;

            object obj = _scmd.ExecuteScalar()
                         ?? throw new ApplicationException("MyDB ExecScalar 실행 결과로 null이 반환되었습니다.");

            return obj;
        }

        private Member ReadMember(SqlDataReader reader)
        {
            int id = int.Parse(reader["m_id"].ToString());
            string email = reader["m_email"].ToString();
            string name = reader["m_name"].ToString();
            DateTime createdAt = DateTime.Parse(reader["m_created_at"].ToString());
            DateTime updatedAt = DateTime.Parse(reader["m_updated_at"].ToString());

            return new Member
            {
                Id = id,
                Email = email,
                Name = name,
                CreatedAt = createdAt.ToString("O"),
                UpdatedAt = updatedAt.ToString("O")
            };
        }

        private DSession ReadSession(SqlDataReader reader)
        {
            int id = int.Parse(reader["s_id"].ToString());
            Member member = ReadMember(reader);
            string token = reader["s_token"].ToString();
            DateTime createdAt = DateTime.Parse(reader["s_created_at"].ToString());
            DateTime expiresAt = DateTime.Parse(reader["s_expires_at"].ToString());

            return new DSession(id, member, token, createdAt, expiresAt);
        }

        private DPost ReadPost(SqlDataReader reader)
        {
            int id = int.Parse(reader["p_id"].ToString());
            Member member = ReadMember(reader);
            string title = reader["p_title"].ToString();
            string content = reader["p_content"].ToString();
            DateTime createdAt = DateTime.Parse(reader["p_created_at"].ToString());
            DateTime updatedAt = DateTime.Parse(reader["p_updated_at"].ToString());

            return new DPost(id, member, title, content, createdAt, updatedAt);
        }

        private DPostFile ReadPostFile(SqlDataReader reader)
        {
            int id = int.Parse(reader["pf_id"].ToString());
            int postId = int.Parse(reader["pf_post_id"].ToString());
            string name = reader["pf_name"].ToString();
            DateTime createdAt = DateTime.Parse(reader["pf_created_at"].ToString());

            return new DPostFile(id, postId, name, createdAt);
        }

        private static string GenerateRandomSessionToken()
        {
            using (RandomNumberGenerator rng = new RNGCryptoServiceProvider())
            {
                byte[] tokenData = new byte[32];
                rng.GetBytes(tokenData);

                return Convert.ToBase64String(tokenData)
                    .Replace('/', '1')
                    .Replace('=', '2')
                    .Replace('+', '3');
            }
        }

        #endregion
    }
}

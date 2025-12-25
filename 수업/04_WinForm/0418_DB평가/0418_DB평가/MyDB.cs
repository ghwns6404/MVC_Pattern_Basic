//MyDB.cs
using _0418_DB평가;
using System;
using System.Collections.Generic;
using System.Data.SqlClient;
using System.Diagnostics;
using System.Security.Cryptography;
using WoosongBit41.Data;

namespace WoosongBit41.DataBase
{
    internal class MyDB
    {
        private const string server_name = "DESKTOP-0I86BTV\\SQLEXPRESS";
        private const string db_name = "wb41";
        private const string sql_id = "aaa";
        private const string sql_pw = "1234";

        private SqlConnection scon = null;
        private SqlCommand scmd = null;

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

        #region 전체 출력(다중 로우 반환 select)
        public List<Student> SelectStudentAll()
        {
            List<Student> students = new List<Student>();

            string sql = string.Format($"select * from Student;");
            using (SqlCommand cmd = new SqlCommand(sql, scon))
            {
                SqlDataReader r = cmd.ExecuteReader();
                while (r.Read())
                {
                    int stu_id      = int.Parse(r["stu_id"].ToString());
                    string stu_name = r["stu_name"].ToString();

                    students.Add(new Student(stu_id, stu_name));
                }
                r.Close();
            }

            return students;
        }
        public List<Subject> SelectSubjectAll()
        {
            List<Subject> subjects = new List<Subject>();

            string sql = string.Format($"select * from Subject;");
            using (SqlCommand cmd = new SqlCommand(sql, scon))
            {
                SqlDataReader r = cmd.ExecuteReader();
                while (r.Read())
                {
                    int sub_id = int.Parse(r["sub_id"].ToString());
                    string sub_name = r["sub_name"].ToString();

                    subjects.Add(new Subject(sub_id, sub_name));
                }
                r.Close();
            }

            return subjects;
        }
        public List<Jumsu> SelectJumsuAll()
        {
            List<Jumsu> jumsus = new List<Jumsu>();

            string sql = string.Format($"select * from Jumsu;");
            using (SqlCommand cmd = new SqlCommand(sql, scon))
            {
                SqlDataReader r = cmd.ExecuteReader();
                while (r.Read())
                {
                    int jumsu_id    = int.Parse(r["jumsu_id"].ToString());
                    int stu_id      = int.Parse(r["stu_id"].ToString());
                    int sub_id      = int.Parse(r["sub_id"].ToString());
                    int jumsu_num   = int.Parse(r["jumsu_num"].ToString());

                    jumsus.Add(new Jumsu(jumsu_id, stu_id, sub_id, jumsu_num));
                }
                r.Close();
            }

            return jumsus;
        }
        #endregion

        #region [메뉴-기능1] 점수 저장 기능
        //insert into Jumsu values(101, 200, 90);
        public bool InsertJumsu(int stu_id, int sub_id, int jumsu)
        {
            string sql = string.Format($"insert into Jumsu values({stu_id}, {sub_id}, {jumsu});");
            return ExSqlCommand(sql);
        }
        #endregion

        #region [메뉴-기능2] 점수 검색 기능
        //select jumsu_num from jumsu where stu_id = 101 and sub_id = 200;;
        public int GetJumsu(int stu_id, int sub_id)
        {
            string sql = string.Format($"select jumsu_num from jumsu where stu_id = {stu_id} and sub_id = {sub_id};");
            object obj = ExSqlScalarCommand(sql);
            if (obj == null)
                throw new Exception("오류 발생");
            return (int)obj;
        }
        #endregion

        #region [메뉴-기능3] 점수 수정 기능
        //update Jumsu set jumsu_num = 95 where stu_id = 101 and sub_id = 200;
        public bool UpdateJumsu(int stu_id, int sub_id, int jumsu)
        {
            string sql = string.Format($"update Jumsu set jumsu_num = {jumsu} where stu_id = {stu_id} and sub_id = {sub_id};");
            return ExSqlCommand(sql);
        }
        #endregion

        #region [메뉴-기능4] 점수 삭제 기능
        //delete Jumsu where stu_id = 101 and sub_id = 200;
        public bool DeleteJumsu(int stu_id, int sub_id)
        {
            string sql = string.Format($"delete Jumsu where stu_id = {stu_id} and sub_id = {sub_id};");
            return ExSqlCommand(sql);
        }
        #endregion

        #region [메뉴-기능5] 점수 검색 기능
        //select stu_name, sub_name, jumsu_num from jumsu , Student, Subject where (Jumsu.stu_id = Student.stu_id) and (Jumsu.sub_id = Subject.sub_id) and (Jumsu.stu_id = 101 and Jumsu.sub_id = 200);
        public string GetJumsuJoin(int stu_id, int sub_id)
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
                    int jumsu_num = int.Parse(r["jumsu_num"].ToString());

                    return string.Format($"{stu_name}%{sub_name}%{jumsu_num}");
                }
            }
            finally
            {
                r.Close();
            }
        }
        #endregion

        #region [메뉴-기능6] 과목명을 입력받아 가장 높은 학생의 이름 출력
        //select stu_name from Jumsu, Student where Jumsu.stu_id = Student.stu_id and  jumsu_num =( select max(jumsu_num)from Jumsu where  sub_id = 201);
        public string GetMaxJumsu(int sub_id)
        {
            string sql = string.Format($"select stu_name from Jumsu, Student where Jumsu.stu_id = Student.stu_id and  jumsu_num =( select max(jumsu_num)from Jumsu where  sub_id = {sub_id});");
            object obj = ExSqlScalarCommand(sql);
            if (obj == null)
                throw new Exception("오류 발생");
            return (string)obj;
        }
        #endregion      

        #region DB 명령 함수(ExecuteNonQuery:I,U,D),(ExcecuteScalar:S),(ExcecuteReader:S)
        private bool ExSqlCommand(string sql)
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

        private object ExSqlScalarCommand(string sql)
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

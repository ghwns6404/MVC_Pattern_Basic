//BookControl.cs
using System;
using System.Collections.Generic;
using WoosongBit41.Data;
using WoosongBit41.DataBase;
using WoosongBit41.Lib;

namespace _0418_DB평가
{
    internal class JumsuControl 
    {
        private MyDB db = new MyDB();

        #region 0. 싱글톤 패턴
        public static JumsuControl Singleton { get; } = null;
        static JumsuControl() { Singleton = new JumsuControl(); }
        private JumsuControl() 
        {
            
        }
        #endregion

        #region 1. 기능 메서드
        public void Jumsu_Insert()
        {
            try
            {
                Console.WriteLine("\n[점수 저장]\n");

                int stu_id  = WbLib.InputInteger("학생 아이디 입력");
                int sub_id  = WbLib.InputInteger("과목 아이디 입력");
                int jumsu   = WbLib.InputInteger("점수 입력");

                if (db.InsertJumsu(stu_id, sub_id, jumsu) == false)
                    throw new Exception("점수 저장 실패");

                Console.WriteLine("점수 저장 성공");
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
            }
        }
        public void PrintAll()
        {
            List<Student> students  = db.SelectStudentAll();
            List<Subject> subjects  = db.SelectSubjectAll();
            List<Jumsu> jumsus      = db.SelectJumsuAll();

            Console.Write("[학생] ");
            foreach (Student student in students)
            {
                Console.Write($"{student.StuId}:{student.StuName} \t");
            }
            Console.WriteLine();

            Console.Write("[과목] ");
            foreach (Subject subject in subjects)
            {
                Console.Write($"{subject.SubId}:{subject.SubName} \t");
            }
            Console.WriteLine();

            Console.Write("[성적] ");
            foreach (Jumsu jumsu in jumsus)
            {
                Console.Write($"{jumsu.JumsuId}-{jumsu.StuId}-{jumsu.SubId}-{jumsu.JumsuNum} \t");
            }
            Console.WriteLine();
        }
        public void Jumsu_Select()
        {
            try
            {
                Console.WriteLine("\n[점수 검색]\n");

                int stu_id = WbLib.InputInteger("학생 아이디 입력");
                int sub_id = WbLib.InputInteger("과목 아이디 입력");

                int jumsu = db.GetJumsu(stu_id , sub_id);
                Console.WriteLine($" -> {jumsu}");
            }
            catch (Exception ex)
            {
                Console.WriteLine("[점수 검색 실패] " + ex.Message);
            }
        }
        public void Jumsu_Update()
        {
            try
            {
                Console.WriteLine("\n[점수 수정]\n");

                int stu_id = WbLib.InputInteger("학생 아이디 입력");
                int sub_id = WbLib.InputInteger("과목 아이디 입력");
                int jumsu  = WbLib.InputInteger("수정할 점수 입력");

                bool b = db.UpdateJumsu(stu_id, sub_id, jumsu);
                if (b == true)
                    Console.WriteLine("수정 되었습니다.");
                else
                    Console.WriteLine("수정 실패");
            }
            catch (Exception ex)
            {
                Console.WriteLine("[점수 수정 실패] " + ex.Message);
            }
        }
        public void Jumsu_Delete()
        {
            try
            {
                Console.WriteLine("\n[점수 삭제]\n");

                int stu_id = WbLib.InputInteger("학생 아이디 입력");
                int sub_id = WbLib.InputInteger("과목 아이디 입력");

                bool b = db.DeleteJumsu(stu_id, sub_id);
                if (b == true)
                    Console.WriteLine("점수가 삭제되었습니다.");
                else
                    Console.WriteLine("점수 삭제 실패");
            }
            catch (Exception ex)
            {
                Console.WriteLine("[점수 삭제 실패] " + ex.Message);
            }
        }
        public void Jumsu_Select_Join()
        {
            try
            {
                Console.WriteLine("\n[점수 검색]\n");

                int stu_id = WbLib.InputInteger("학생 아이디 입력");
                int sub_id = WbLib.InputInteger("과목 아이디 입력");

                string str = db.GetJumsuJoin(stu_id, sub_id);

                string[] sp = str.Split('%');

                string stu_name = sp[0].ToString();
                string sub_name = sp[1].ToString();
                int jumsu = int.Parse(sp[2].ToString());
                Console.WriteLine($"{stu_name} \t {sub_name} \t {jumsu}점");
            }
            catch (Exception ex)
            {
                Console.WriteLine("[점수 검색 실패] " + ex.Message);
            }
        }
        public void Jumsu_Max()
        {
            try
            {
                Console.WriteLine("\n[과목에 따른 최대 점수 검색]\n");

                int sub_id = WbLib.InputInteger("과목 아이디 입력");

                string stu_name = db.GetMaxJumsu(sub_id);
                Console.WriteLine($"점수가 가장 높은 학생 : {stu_name}");
            }
            catch (Exception ex)
            {
                Console.WriteLine("[점수 검색 실패] " + ex.Message);
            }
        }
        #endregion 

        #region 2. 시작과 종료 메서드(DB 연결 및 종료)
        public void Init()
        {
            db.Connect();
        }
        public void Exit()
        {
            db.Close();
        }
        #endregion
    }
}
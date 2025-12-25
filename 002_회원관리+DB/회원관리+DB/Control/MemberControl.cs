//MemberControl.cs

using System;
using System.Collections.Generic;



namespace 회원관리_DB
{
    internal class MemberControl
    {
        #region 싱글톤
        public static MemberControl Singleton { get; } = null;
        static MemberControl() { Singleton = new MemberControl(); }
        private MemberControl() { }
        #endregion

        private MyDB db = new MyDB();

        #region 기본 C R U D
        public void Control_Insert()
        {
            Console.WriteLine("[추가] 회원정보 입력");
            try
            {
                int id = WbLib.InputInteger("회원번호");
                string name = WbLib.InputString("이름");
                int age = WbLib.InputInteger("나이");
                string phone = WbLib.InputString("전화번호");
                string addr = WbLib.InputString("주소");

                bool b = db.DB_Insert(id, name, age, phone, addr);
                if (b == false)
                {
                    throw new Exception();
                }
                else
                {
                    Console.WriteLine($"[ {id}, {name}, {age}, {phone}, {addr} ] 회원이 저장되었습니다.");
                }
            }
            catch(Exception ex)
            {
                Console.WriteLine(ex.Message);
            }
            
        }
        public void Control_Select()
        {
            Console.WriteLine("[수정] 회원ID로 검색\n");
            try
            {
                int id = WbLib.InputInteger("ID를 입력하세요");

                string str = db.DB_Select(id);
                string[] sp = str.Split('%');

                int _id = int.Parse(sp[0].ToString());
                string name = sp[1].ToString();
                int age = int.Parse(sp[2].ToString());
                string phone = sp[3].ToString();
                string addr = sp[4].ToString();

                Console.WriteLine($"\nID : [{_id}] 의 정보\n이름 : {name}\n나이 : {age}\n전화 : {phone}\n주소 : {addr}");
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
            }
        }
        public void Control_Update()
        {
            Console.WriteLine("[수정] 회원ID로 수정 가능\n");
            try
            {
                int id = WbLib.InputInteger("ID를 입력하세요");
                string name = WbLib.InputString("[수정할] 이름");
                int age = WbLib.InputInteger("[수정할] 나이");
                string phone = WbLib.InputString("[수정할] 전화번호");
                string addr = WbLib.InputString("[수정할] 주소");

                db.DB_Update(id, name, age, phone, addr);
                Console.WriteLine($"[ {id}이 {name}, {age}, {phone}, {addr} ] 으로 수정되었습니다.");
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
            }

        }
        public void Control_Delete()
        {
            try
            {
                Console.WriteLine("[삭제] 회원ID 입력 :");

                int id = WbLib.InputInteger("회원번호");
                db.DB_Delete(id);
                Console.WriteLine($"[ {id}번이 삭제되었습니다.");
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
            }
        }
        #endregion

        #region 추가 C R U D
        public void Control_DeleteAll()
        {
            Console.WriteLine("### WARNING : 회원 데이터 초기화! ###");
            Console.WriteLine("정말 초기화 하시겠습니까? ( Y / N ) \n");
            
            string input = Console.ReadLine();
            if (input == "y" || input == "Y")
            {
                db.DB_DeleteAll();
                Console.WriteLine("\n\n데이터가 전부 초기화 되었습니다");
            }
            else if (input == "n" || input == "N")
            {
                Console.WriteLine("초기화 취소");
            }
            else
            {
                Console.WriteLine("잘못된 입력입니다....");
            }
        }
        public void Control_PrintAll()
        {
            try
            {
                List<Member> members = db.DB_PrintAll();

                foreach (Member mem in members)
                {
                    Console.Write($"회원번호:{mem.Id}\t이름:{mem.Name}\t나이:{mem.Age}\t폰:{mem.Phone}\t주소:{mem.Addr}\n\n");
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
            }
        }
        #endregion

        #region Connect / Close
        public void DB_Connect()
        {
            db.Connect();
        }
        public void DB_Close()
        {
            db.Close();
        }
        #endregion
    }
}

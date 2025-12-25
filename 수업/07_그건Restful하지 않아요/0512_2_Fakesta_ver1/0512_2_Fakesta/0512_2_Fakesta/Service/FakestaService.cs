// FakestaService.cs

using System;
using System.Collections.Generic;
using System.Net;
using System.Runtime.InteropServices.WindowsRuntime;
using System.ServiceModel.Web;
using _0512_2_Fakesta.Data;

namespace _0512_2_Fakesta.Service
{
    internal class FakestaService : IFakesta
    {
        DateTime date = DateTime.Now;
       

        private MyDB mydb = MyDB.Singleton;
        public List<Member> GetMembers()//ㅇㅋ
        {
            return mydb.db_Select_Member_All();
        }

        public Member GetMember(string id)//ㅇㅋㅋ
        {
            try
            {
                return mydb.db_Select_Member_Id(int.Parse(id));
            }
            catch (Exception ex)
            {
                Console.WriteLine("에러: " + ex.Message);
                WebOperationContext.Current.OutgoingResponse.StatusCode = HttpStatusCode.InternalServerError;   //상태값
                return null;
            }
        }

        public void InsertMember(string email, string pw, string name)//ㅇㅋ
        {
            try
            {
                Console.WriteLine("InsertMember: email: {0}\npw: {1}\nname: {2}", email, pw, name);

                // 이거 응답 본문 없이 할 때 쓰세요. (응답코드 204)
                WebOperationContext.Current.OutgoingResponse.StatusCode = HttpStatusCode.NoContent;
                mydb.db_Insert_Member(name, email, pw);
            }
            catch (Exception ex)
            {
                Console.WriteLine("에러: " + ex.Message);
                WebOperationContext.Current.OutgoingResponse.StatusCode = HttpStatusCode.InternalServerError;   //상태값
            }
        }

        public CreateSessionResponse LoginMember(string email, string pw) //@@@@@@@@@
        {
            CreateSessionResponse session = new CreateSessionResponse();
            try 
            {
                Console.WriteLine("LoginMember: email: {0}\npw: {1}", email, pw);

                session.Token = mydb.db_Login(email, pw); // 로그인 성공시 세션 테이블에 랜덤 토큰 추가
                return session; 
            }
            catch (Exception ex)
            {
                Console.WriteLine("에러: " + ex.Message);
                WebOperationContext.Current.OutgoingResponse.StatusCode = HttpStatusCode.InternalServerError;   //상태값
                return null;
            }

        }


        public List<Post> GetPosts()//ㅇㅋ
        {
            return mydb.db_Select_Post_All();
        }

        public Post GetPost(string id)//ㅇㅋ
        {
            try
            {
                return mydb.db_Select_Post_title(int.Parse(id));
            }
            catch (Exception ex)
            {
                Console.WriteLine("에러: " + ex.Message);
                WebOperationContext.Current.OutgoingResponse.StatusCode = HttpStatusCode.InternalServerError;   //상태값
                return null;
            }
        }

        public void InsertPost(string title, string filename) //@@@@@@@@@
        {
            try
            {
                
                // 이건 요청 헤더 가져올 때죠.
                string token = WebOperationContext.Current.IncomingRequest.Headers.Get("Authorization");

                Console.WriteLine("InsertPost: token: {0}\ntitle: {1}\nfilename: {2} date: {3}", token, title, filename, date);

                WebOperationContext.Current.OutgoingResponse.StatusCode = HttpStatusCode.NoContent;
                mydb.db_Insert_Post(token, title, filename, "random-path", date); // path는 나중에 추가할 것임. (지금은 빈 문자열로 대체함.)
            }
            catch (Exception ex)
            {
                Console.WriteLine("에러: " + ex.Message);
                WebOperationContext.Current.OutgoingResponse.StatusCode = HttpStatusCode.InternalServerError;   //상태값
            }   

        }

        public void DeletePost(string id)
        {
            try
            {
                string token = WebOperationContext.Current.IncomingRequest.Headers.Get("Authorization");

                Console.WriteLine("DeletePost: token: {0}\nid: {1}", token, id);

                mydb.db_Delete_Post(int.Parse(id));

                WebOperationContext.Current.OutgoingResponse.StatusCode = HttpStatusCode.NoContent;
            }
            catch (Exception ex)
            {
                Console.WriteLine("에러: " + ex.Message);
                WebOperationContext.Current.OutgoingResponse.StatusCode = HttpStatusCode.InternalServerError;   //상태값
            }

        }
    }
}

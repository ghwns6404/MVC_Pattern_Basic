// FakestaService.cs

using System;
using System.Collections.Generic;
using System.IO;
using System.Net;
using System.Runtime.InteropServices.WindowsRuntime;
using System.ServiceModel.Web;
using _0512_2_Fakesta.Data;

namespace _0512_2_Fakesta.Service
{
    internal class FakestaService : IFakesta
    {
        DateTime created_at = DateTime.Now;
        DateTime updated_at = DateTime.Now;
        DateTime expires_at = DateTime.Now.AddMinutes(30); // 30분 후 만료  

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
                Console.WriteLine("InsertMember: email: {0}\npw: {1}\nname: {2} created_at {3} updated_at {4}", email, pw, name, created_at, updated_at);

                // 이거 응답 본문 없이 할 때 쓰세요. (응답코드 204)
                WebOperationContext.Current.OutgoingResponse.StatusCode = HttpStatusCode.NoContent;
                mydb.db_Insert_Member(name, email, pw, created_at, updated_at);
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
                Console.WriteLine("LoginMember: email: {0}\npw: {1} create_at: {2} expires_at: {3}", email, pw, created_at, expires_at);

                session.Token = mydb.db_Login(email, pw, created_at, expires_at); //생성일, 만료일
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

        public Post GetPost(string id)
        {
            try
            {
                Post post = mydb.db_Select_Post_title(int.Parse(id)); 
                return post;
            }
            catch (Exception ex)
            {
                Console.WriteLine("에러: " + ex.Message);
                WebOperationContext.Current.OutgoingResponse.StatusCode = HttpStatusCode.InternalServerError; // 상태값
                return null;
            }
        }

        public void InsertPost(string name, string content) //@@@@@@@@@
        {
            try
            {
                // 이건 요청 헤더 가져올 때죠.
                string token = WebOperationContext.Current.IncomingRequest.Headers.Get("Authorization");

                Console.WriteLine("InsertPost: token: {0}\ntitle: {1}\nfilename: {2}\ncreatedate: {3}\nupdatedate:{4}", token, name, content, created_at, updated_at);

                WebOperationContext.Current.OutgoingResponse.StatusCode = HttpStatusCode.NoContent;
                mydb.db_Insert_Post(token, name, content); 
            }
            catch (Exception ex)
            {
                Console.WriteLine("에러: " + ex.Message);
                WebOperationContext.Current.OutgoingResponse.StatusCode = HttpStatusCode.InternalServerError; //상태값
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

        public Member GetMemberMe()
        {
            throw new NotImplementedException();
        }

        public void UpdateMemberPw(string pw, string newPw)
        {
            throw new NotImplementedException();
        }

        public void DeleteMemberMe()
        {
            throw new NotImplementedException();
        }

        public CreateSessionResponse CreateSessionMe(string email, string pw)
        {
            throw new NotImplementedException();
        }

        public Session GetSessionWhoami()
        {
            throw new NotImplementedException();
        }

        public List<Session> GetSessionMe()
        {
            throw new NotImplementedException();
        }

        public Stream GetPostFile(string id)
        {
            throw new NotImplementedException();
        }
    }
}

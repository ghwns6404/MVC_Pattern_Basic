// FakestaService.cs

using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net;
using System.ServiceModel.Web;
using System.Xml.Linq;
using _0512_2_Fakesta.Data;
using HttpMultipartParser;


namespace _0512_2_Fakesta.Service
{
    internal class FakestaService : IFakesta
    {
        DateTime created_at = DateTime.Now;
        DateTime updated_at = DateTime.Now;
        DateTime expires_at = DateTime.Now.AddMinutes(30); // 30분 후 만료  

        private MyDB mydb = MyDB.Singleton;

        #region  ver2
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
        public void InsertPost(Stream stream)
        {
            string token = WebOperationContext.Current.IncomingRequest.Headers.Get("Authorization");

            var parser = MultipartFormDataParser.Parse(stream);
            string title = parser.GetParameterValue("title");
            string content = parser.GetParameterValue("content"); 

            Console.WriteLine("InsertPost\n  token: {0}\n  title: {1}", token, title);

            var files = new List<(string name, byte[] content)>();

            foreach (var file in parser.Files)
            {
                string fileName = file.FileName;
                byte[] data;

                using (var ms = new MemoryStream())
                {
                    file.Data.CopyTo(ms);
                    data = ms.ToArray();
                }

                Console.WriteLine("  >> 파일 업로드: {0}, 크기: {1} bytes", fileName, data.Length);

                files.Add((fileName, data));
            }

            mydb.db_Insert_Post(token, title, content, files);
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
        #endregion

        #region ver3            
        public Member GetMemberMe()
        {
            try
            {
                //select문으로 DB에 저장된 회원정보를 찾아서 로그인상태인지 확인?
                string token = WebOperationContext.Current.IncomingRequest.Headers.Get("Authorization");
                Console.WriteLine("GetMemberMe: token: {0}", token);
                Member member = mydb.db_Select_Member_Me(token);
                if (member == null)
                {
                    WebOperationContext.Current.OutgoingResponse.StatusCode = HttpStatusCode.Unauthorized; //상태값
                    return null;
                }
                else
                {
                    WebOperationContext.Current.OutgoingResponse.StatusCode = HttpStatusCode.OK; //상태값
                    return member;
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine("에러: " + ex.Message);
                WebOperationContext.Current.OutgoingResponse.StatusCode = HttpStatusCode.InternalServerError; // 상태값
                return null;
            }
        }

        public void UpdateMemberPw(string pw, string newPw)
        {
            try
            {
                string token = WebOperationContext.Current.IncomingRequest.Headers.Get("Authorization");

                Console.WriteLine("UpdateMemberPw: token: {0}\npw: {1}\nnewPw: {2}", token, pw, newPw);
                mydb.db_Update_Member_Pw(token, pw, newPw);
                WebOperationContext.Current.OutgoingResponse.StatusCode = HttpStatusCode.NoContent;
            }
            catch (Exception ex)
            {
                Console.WriteLine("에러: " + ex.Message);
                WebOperationContext.Current.OutgoingResponse.StatusCode = HttpStatusCode.InternalServerError;   //상태값
            }
        }

        public void DeleteMemberMe()
        {
            try
            {
                string token = WebOperationContext.Current.IncomingRequest.Headers.Get("Authorization");

                //Console.WriteLine("DeletePost: token: {0}\nid: {1}", token);

                mydb.db_Delete_Member(token);

                WebOperationContext.Current.OutgoingResponse.StatusCode = HttpStatusCode.NoContent;
            }
            catch (Exception ex)
            {
                Console.WriteLine("에러: " + ex.Message);
                WebOperationContext.Current.OutgoingResponse.StatusCode = HttpStatusCode.InternalServerError;   //상태값
            }
        }

        public CreateSessionResponse CreateSessionMe(string email, string pw)//로그인
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
                WebOperationContext.Current.OutgoingResponse.StatusCode = HttpStatusCode.Unauthorized;   //상태값
                return null;
            }
        }

        public Session GetSessionWhoami()
        {
            try
            {
                string token = WebOperationContext.Current.IncomingRequest.Headers.Get("Authorization");
                Console.WriteLine("GetSessionWhoami: token: {0}", token);
                Session session = mydb.db_Select_Session_Whoami(token);


                if (session == null)
                {
                    WebOperationContext.Current.OutgoingResponse.StatusCode = HttpStatusCode.Unauthorized; //상태값
                    return null;
                }
                else
                {
                    WebOperationContext.Current.OutgoingResponse.StatusCode = HttpStatusCode.OK; //상태값
                    return session;
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine("에러: " + ex.Message);
                WebOperationContext.Current.OutgoingResponse.StatusCode = HttpStatusCode.InternalServerError; // 상태값
                return null;
            }
        }

        public List<Session> GetSessionMe()
        {
            try
            {
                string token = WebOperationContext.Current.IncomingRequest.Headers.Get("Authorization");
                Console.WriteLine("GetSessionMe: token: {0}", token);
                List<Session> session = mydb.db_Select_Session_Me(token);
                if (session == null)
                {
                    WebOperationContext.Current.OutgoingResponse.StatusCode = HttpStatusCode.Unauthorized; //상태값
                    return null;
                }
                else
                {
                    WebOperationContext.Current.OutgoingResponse.StatusCode = HttpStatusCode.OK; //상태값
                    return session;
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine("에러: " + ex.Message);
                WebOperationContext.Current.OutgoingResponse.StatusCode = HttpStatusCode.InternalServerError; // 상태값
                return null;
            }
        }

        public Stream GetPostFile(string id)
        {
            try
            {
                Stream stream = mydb.db_Select_Post_FileId(int.Parse(id));

                if (stream == null)
                {
                    WebOperationContext.Current.OutgoingResponse.StatusCode = HttpStatusCode.NotFound;
                    return null;
                }

                WebOperationContext.Current.OutgoingResponse.ContentType = "image/jpg";
                return stream;
            }
            catch (Exception ex)
            {
                Console.WriteLine("에러: " + ex.Message);
                WebOperationContext.Current.OutgoingResponse.StatusCode = HttpStatusCode.InternalServerError;
                return null;
            }
        }


        public void DeleteSession(string id)
        {
            try
            {
                string token = WebOperationContext.Current.IncomingRequest.Headers.Get("Authorization");

                Console.WriteLine("DeletePost: token: {0}\nid: {1}", token, id);

                mydb.db_Delete_Session(int.Parse(id));

                WebOperationContext.Current.OutgoingResponse.StatusCode = HttpStatusCode.NoContent;
            }
            catch (Exception ex)
            {
                Console.WriteLine("에러: " + ex.Message);
                WebOperationContext.Current.OutgoingResponse.StatusCode = HttpStatusCode.InternalServerError;   //상태값
            }
        }
        #endregion
    }
}

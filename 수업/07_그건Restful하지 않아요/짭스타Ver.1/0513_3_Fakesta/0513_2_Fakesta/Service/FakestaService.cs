// FakestaService.cs

using System;
using System.Collections.Generic;
using System.Configuration;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;
using System.Linq;
using System.Net;
using System.ServiceModel;
using System.ServiceModel.Web;
using _0513_2_Fakesta.Data;
using _0513_2_Fakesta.Database;
using _0513_2_Fakesta.DBData;
using _0513_2_Fakesta.Error;
using HttpMultipartParser;

namespace _0513_2_Fakesta.Service
{
    [ServiceBehavior(ConcurrencyMode = ConcurrencyMode.Single, InstanceContextMode = InstanceContextMode.Single)]
    internal class FakestaService : IFakesta
    {
        private readonly MyDB _db = MyDB.Instance;

        private readonly string _filePath = ConfigurationManager.AppSettings["FilePath"].TrimEnd('/');

        public List<PublicMember> GetMembers()
        {
            try
            {
                Console.WriteLine("GetMembers");

                return _db.SelectAllMember()
                    .Select(m => m.ToPublic())
                    .ToList();
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);

                throw new WebFaultException<string>("알 수 없는 서버 오류가 발생했습니다!", HttpStatusCode.InternalServerError);
            }
        }

        public PublicMember GetMember(string id)
        {
            try
            {
                Console.WriteLine("GetMember\n  id: {0}", id);

                return _db.SelectMemberById(int.Parse(id)).ToPublic();
            }
            catch (FakestaMemberNotFoundException ex)
            {
                Console.WriteLine(ex.Message);

                throw new WebFaultException<string>(ex.Message, HttpStatusCode.NotFound);
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);

                throw new WebFaultException<string>("알 수 없는 서버 오류가 발생했습니다!", HttpStatusCode.InternalServerError);
            }
        }

        public Member GetMemberMe()
        {
            try
            {
                DSession session = GetCurrentSession();

                Console.WriteLine("GetMemberMe\n  session id: {0}", session.Id);

                return session.Member;
            }
            catch (FakestaAuthException ex)
            {
                Console.WriteLine(ex.Message);

                throw new WebFaultException<string>(ex.Message, HttpStatusCode.Unauthorized);
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);

                throw new WebFaultException<string>("알 수 없는 서버 오류가 발생했습니다!", HttpStatusCode.InternalServerError);
            }
        }

        public void InsertMember(string email, string pw, string name)
        {
            try
            {
                Console.WriteLine("InsertMember\n  email: {0}\n  pw: {1}\n  name: {2}", email, pw, name);

                int id = _db.InsertMember(email, pw, name);

                UriTemplateMatch match = WebOperationContext.Current.IncomingRequest.UriTemplateMatch;
                UriTemplate template = new UriTemplate("/members/{id}");
                WebOperationContext.Current.OutgoingResponse.SetStatusAsCreated(
                    template.BindByPosition(match.BaseUri, id.ToString()));
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);

                throw new WebFaultException<string>("알 수 없는 서버 오류가 발생했습니다!", HttpStatusCode.InternalServerError);
            }
        }

        public void UpdateMemberPw(string pw, string newPw)
        {
            // 1. 세션을 가져온다.
            DSession session;
            try
            {
                session = GetCurrentSession();

                Console.WriteLine("UpdateMemberPw\n  session id: {0}\n  pw: {1}\n  newPw: {2}", session.Id, pw, newPw);
            }
            catch (FakestaAuthException ex)
            {
                Console.WriteLine(ex.Message);

                throw new WebFaultException<string>(ex.Message, HttpStatusCode.Unauthorized);
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);

                throw new WebFaultException<string>("알 수 없는 서버 오류가 발생했습니다!", HttpStatusCode.InternalServerError);
            }

            // 2. 기존 비밀번호 확인 및 변경
            try
            {
                _db.CheckMemberPw(session.Member.Id, pw);

                _db.UpdateMemberPw(session.Member.Id, newPw);

                WebOperationContext.Current.OutgoingResponse.StatusCode = HttpStatusCode.NoContent;
            }
            catch (FakestaAuthException ex)
            {
                Console.WriteLine(ex.Message);

                throw new WebFaultException<string>(ex.Message, HttpStatusCode.Forbidden);
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);

                throw new WebFaultException<string>("알 수 없는 서버 오류가 발생했습니다!", HttpStatusCode.InternalServerError);
            }
        }

        public void DeleteMemberMe()
        {
            try
            {
                DSession session = GetCurrentSession();

                Console.WriteLine("DeleteMemberMe\n  session id: {0}", session.Id);

                _db.DeleteMember(session.Member.Id);

                WebOperationContext.Current.OutgoingResponse.StatusCode = HttpStatusCode.NoContent;
            }
            catch (FakestaAuthException ex)
            {
                Console.WriteLine(ex.Message);

                throw new WebFaultException<string>(ex.Message, HttpStatusCode.Unauthorized);
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);

                throw new WebFaultException<string>("알 수 없는 서버 오류가 발생했습니다!", HttpStatusCode.InternalServerError);
            }
        }


        public CreateSessionResponse CreateSessionMe(string email, string pw)
        {
            WebOperationContext.Current.OutgoingResponse.Headers.Add("Access-Control-Allow-Origin", "*");

            try
            {
                Console.WriteLine("LoginMember\n  email: {0}\n  pw: {1}", email, pw);

                Member member = _db.CheckMemberPw(email, pw);

                (int sessionId, string token) = _db.InsertSession(member.Id);

                return new CreateSessionResponse { Token = token };
            }
            catch (FakestaAuthException ex)
            {
                Console.WriteLine(ex.Message);

                throw new WebFaultException<string>(ex.Message, HttpStatusCode.Unauthorized);
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);

                throw new WebFaultException<string>("알 수 없는 서버 오류가 발생했습니다!", HttpStatusCode.InternalServerError);
            }
        }

        public Session GetSessionWhoami()
        {
            try
            {
                DSession session = GetCurrentSession();

                Console.WriteLine("GetSessionWhoami\n  session id: {0}", session.Id);

                return session.ToSession();
            }
            catch (FakestaAuthException ex)
            {
                Console.WriteLine(ex.Message);

                throw new WebFaultException<string>(ex.Message, HttpStatusCode.Unauthorized);
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);

                throw new WebFaultException<string>("알 수 없는 서버 오류가 발생했습니다!", HttpStatusCode.InternalServerError);
            }
        }

        public List<Session> GetSessionMe()
        {
            try
            {
                DSession session = GetCurrentSession();

                Console.WriteLine("GetSessionMe\n  session id: {0}", session.Id);

                List<DSession> sessions = _db.SelectAllMemberSession(session.Member.Id);

                return sessions.FindAll(s => s.Id != session.Id)
                    .Select(s => s.ToSession())
                    .ToList();
            }
            catch (FakestaAuthException ex)
            {
                Console.WriteLine(ex.Message);

                throw new WebFaultException<string>(ex.Message, HttpStatusCode.Unauthorized);
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);

                throw new WebFaultException<string>("알 수 없는 서버 오류가 발생했습니다!", HttpStatusCode.InternalServerError);
            }
        }

        public void DeleteSession(string id)
        {
            try
            {
                DSession session = GetCurrentSession();

                Console.WriteLine("DeleteSession\n  session id: {0}", session.Id);

                _db.DeleteSession(int.Parse(id), session.Member.Id);

                WebOperationContext.Current.OutgoingResponse.StatusCode = HttpStatusCode.NoContent;
            }
            catch (FakestaAuthException ex)
            {
                Console.WriteLine(ex.Message);

                throw new WebFaultException<string>(ex.Message, HttpStatusCode.Unauthorized);
            }
            // 세션을 존재하지 않거나, 해당 회원의 세션이 아니면 404 오류
            catch (FakestaSessionNotFoundException ex)
            {
                Console.WriteLine(ex.Message);

                throw new WebFaultException<string>(ex.Message, HttpStatusCode.NotFound);
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);

                throw new WebFaultException<string>("알 수 없는 서버 오류가 발생했습니다!", HttpStatusCode.InternalServerError);
            }
        }


        public List<Post> GetPosts()
        {
            try
            {
                Console.WriteLine("GetPosts");

                return _db.SelectAllPost()
                    .Select(p => p.ToPost())
                    .ToList();
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);

                throw new WebFaultException<string>("알 수 없는 서버 오류가 발생했습니다!", HttpStatusCode.InternalServerError);
            }
        }

        public Post GetPost(string id)
        {
            try
            {
                Console.WriteLine("GetPost\n  id: {0}", id);

                return _db.SelectPostById(int.Parse(id)).ToPost();
            }
            catch (FakestaPostNotFoundException ex)
            {
                Console.WriteLine(ex.Message);

                throw new WebFaultException<string>(ex.Message, HttpStatusCode.NotFound);
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);

                throw new WebFaultException<string>("알 수 없는 서버 오류가 발생했습니다!", HttpStatusCode.InternalServerError);
            }
        }

        public void InsertPost(Stream stream)
        {
            try
            {
                DSession session = GetCurrentSession();

                var parser = MultipartFormDataParser.Parse(stream);
                string title = parser.GetParameterValue("title");
                string content = parser.GetParameterValue("content");
                // 파일이 여러 개일 수도 있어요.
                var files = parser.Files;

                Console.WriteLine("InsertPost\n  session id: {0}\n  title: {1}\n  content: {2}\n  files.Count: {3}", session.Id, title, content, files.Count);

                if (files.Count < 1 || files.Count > 50)
                    throw new ArgumentOutOfRangeException("게시물 파일이 1개보다 적거나 50개를 초과할 수 없습니다.");


                int postId = _db.InsertPost(session.Member.Id, title, content);

                foreach (var file in files)
                {
                    _db.InsertPostFile(postId, file.FileName, _filePath, file.Data);
                }

                UriTemplateMatch match = WebOperationContext.Current.IncomingRequest.UriTemplateMatch;
                UriTemplate template = new UriTemplate("/posts/{id}");
                WebOperationContext.Current.OutgoingResponse.SetStatusAsCreated(template.BindByPosition(match.BaseUri, postId.ToString()));
            }
            catch (FakestaAuthException ex)
            {
                Console.WriteLine(ex.Message);

                throw new WebFaultException<string>(ex.Message, HttpStatusCode.Unauthorized);
            }
            catch (ArgumentOutOfRangeException ex)
            {
                Console.WriteLine(ex.Message);

                throw new WebFaultException<string>(ex.Message, HttpStatusCode.BadRequest);
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);

                throw new WebFaultException<string>("알 수 없는 서버 오류가 발생했습니다!", HttpStatusCode.InternalServerError);
            }
        }

        public void DeletePost(string id)
        {
            try
            {
                DSession session = GetCurrentSession();

                Console.WriteLine("DeletePost\n  session id: {0}\n  id: {1}", session.Id, id);

                int postId = int.Parse(id);
                var files = _db.SelectAllPostFileByPostId(postId);
                foreach (var file in files)
                {
                    try
                    {
                        File.Delete($"{_filePath}/post-files/{file.Id}.png");
                    }
                    catch (Exception ex)
                    {
                        Console.WriteLine(ex.Message);
                    }
                }

                _db.DeletePost(postId, session.Member.Id);

                WebOperationContext.Current.OutgoingResponse.StatusCode = HttpStatusCode.NoContent;
            }
            catch (FakestaAuthException ex)
            {
                Console.WriteLine(ex.Message);

                throw new WebFaultException<string>(ex.Message, HttpStatusCode.Unauthorized);
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);

                throw new WebFaultException<string>("알 수 없는 서버 오류가 발생했습니다!", HttpStatusCode.InternalServerError);
            }
        }


        public Stream GetPostFile(string id)
        {
            try
            {
                Console.WriteLine("GetPostFile\n  id: {0}", id);

                DPostFile postFile = _db.SelectPostFileById(int.Parse(id));

                MemoryStream ms = new MemoryStream();
                using (Image image = Image.FromFile($"{_filePath}/post-files/{postFile.Id}.png"))
                {
                    image.Save(ms, ImageFormat.Png);
                    ms.Position = 0;

                    WebOperationContext.Current.OutgoingResponse.ContentType = "image/png";
                    return ms;
                }
            }
            catch (FakestaPostFileNotFoundException ex)
            {
                Console.WriteLine(ex.Message);

                throw new WebFaultException<string>(ex.Message, HttpStatusCode.NotFound);
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);

                throw new WebFaultException<string>("알 수 없는 서버 오류가 발생했습니다!", HttpStatusCode.InternalServerError);
            }
        }


        // Helper

        private DSession GetCurrentSession()
        {
            string token = WebOperationContext.Current.IncomingRequest.Headers.Get("Authorization");
            if (string.IsNullOrEmpty(token))
                throw new FakestaAuthException("로그인이 필요한 기능입니다.");

            try
            {
                return _db.SelectSessionByToken(token);
            }
            catch (FakestaSessionNotFoundException ex)
            {
                throw new FakestaAuthException("유효하지 않은 세션입니다.");
            }
        }
    }
}

// IFakesta.cs

using System;
using System.Collections.Generic;
using System.IO;
using System.ServiceModel;
using System.ServiceModel.Web;
using _0512_2_Fakesta.Data;

namespace _0512_2_Fakesta.Service
{
    [ServiceContract]
    internal interface IFakesta
    {
        #region ver2
        [OperationContract]
        [WebGet(UriTemplate = "/members", ResponseFormat = WebMessageFormat.Json)]
        List<Member> GetMembers();

        [OperationContract]
        [WebGet(UriTemplate = "/members/{id}", ResponseFormat = WebMessageFormat.Json)]
        Member GetMember(string id);

        [OperationContract]
        [WebInvoke(Method = "POST", UriTemplate = "/members", BodyStyle = WebMessageBodyStyle.Wrapped, RequestFormat = WebMessageFormat.Json)]
        void InsertMember(string email, string pw, string name);

        [OperationContract]
        [WebInvoke(Method = "POST", UriTemplate = "/members/login", BodyStyle = WebMessageBodyStyle.WrappedRequest, RequestFormat = WebMessageFormat.Json, ResponseFormat = WebMessageFormat.Json)]
        CreateSessionResponse LoginMember(string email, string pw);


        [OperationContract]
        [WebGet(UriTemplate = "/posts", ResponseFormat = WebMessageFormat.Json)]
        List<Post> GetPosts();

        [OperationContract]
        [WebGet(UriTemplate = "/posts/{id}", ResponseFormat = WebMessageFormat.Json)]
        Post GetPost(string id);

        [OperationContract]
        [WebInvoke(Method = "POST", UriTemplate = "/posts", BodyStyle = WebMessageBodyStyle.Wrapped, RequestFormat = WebMessageFormat.Json)]
        void InsertPost(string name, string content);

        [OperationContract]
        [WebInvoke(Method = "DELETE", UriTemplate = "/posts/{id}")]
        void DeletePost(string id);
        #endregion

        #region ver3
        [OperationContract]//¡Ú¡Ú¡Ú¡Ú
        [WebGet(UriTemplate = "/members/me", ResponseFormat = WebMessageFormat.Json)]
        Member GetMemberMe();

        [OperationContract]//¡Ú¡Ú¡Ú¡Ú
        [WebInvoke(Method = "PUT", UriTemplate = "/members/me/pw", BodyStyle = WebMessageBodyStyle.WrappedRequest, RequestFormat = WebMessageFormat.Json, ResponseFormat = WebMessageFormat.Json)]
        void UpdateMemberPw(string pw, string newPw);

        [OperationContract]//¡Ú¡Ú¡Ú¡Ú
        [WebInvoke(Method = "DELETE", UriTemplate = "/members/me", ResponseFormat = WebMessageFormat.Json)]
        void DeleteMemberMe();

        [OperationContract]//¡Ú¡Ú¡Ú¡Ú
        [WebInvoke(Method = "POST", UriTemplate = "/sessions/me", BodyStyle = WebMessageBodyStyle.WrappedRequest, RequestFormat = WebMessageFormat.Json, ResponseFormat = WebMessageFormat.Json)]
        CreateSessionResponse CreateSessionMe(string email, string pw);

        [OperationContract]//¡Ú¡Ú¡Ú¡Ú
        [WebGet(UriTemplate = "/sessions/whoami", ResponseFormat = WebMessageFormat.Json)]
        Session GetSessionWhoami();

        [OperationContract]//¡Ú¡Ú¡Ú¡Ú
        [WebGet(UriTemplate = "/sessions/me", ResponseFormat = WebMessageFormat.Json)]
        List<Session> GetSessionMe();

        [OperationContract]//¡Ú¡Ú¡Ú¡Ú
        [WebGet(UriTemplate = "/post-files/{id}", ResponseFormat = WebMessageFormat.Json)]
        Stream GetPostFile(string id);

        #endregion
    }
}

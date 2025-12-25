// IFakesta.cs

using System.Collections.Generic;
using System.IO;
using System.ServiceModel;
using System.ServiceModel.Web;
using _0513_2_Fakesta.Data;

namespace _0513_2_Fakesta.Service
{
    [ServiceContract]
    internal interface IFakesta
    {
        // # Members

        [OperationContract]
        [WebGet(UriTemplate = "/members", ResponseFormat = WebMessageFormat.Json)]
        List<PublicMember> GetMembers();

        [OperationContract]
        [WebGet(UriTemplate = "/members/{id}", ResponseFormat = WebMessageFormat.Json)]
        PublicMember GetMember(string id);

        [OperationContract]
        [WebGet(UriTemplate = "/members/me", ResponseFormat = WebMessageFormat.Json)]
        Member GetMemberMe();

        [OperationContract]
        [WebInvoke(Method = "POST", UriTemplate = "/members", BodyStyle = WebMessageBodyStyle.WrappedRequest, RequestFormat = WebMessageFormat.Json, ResponseFormat = WebMessageFormat.Json)]
        void InsertMember(string email, string pw, string name);

        [OperationContract]
        [WebInvoke(Method = "PUT", UriTemplate = "/members/me/pw", BodyStyle = WebMessageBodyStyle.WrappedRequest, RequestFormat = WebMessageFormat.Json, ResponseFormat = WebMessageFormat.Json)]
        void UpdateMemberPw(string pw, string newPw);

        [OperationContract]
        [WebInvoke(Method = "DELETE", UriTemplate = "/members/me", ResponseFormat = WebMessageFormat.Json)]
        void DeleteMemberMe();

        // # Sessions

        [OperationContract]
        [WebInvoke(Method = "POST", UriTemplate = "/sessions/me", BodyStyle = WebMessageBodyStyle.WrappedRequest, RequestFormat = WebMessageFormat.Json, ResponseFormat = WebMessageFormat.Json)]
        CreateSessionResponse CreateSessionMe(string email, string pw);

        [OperationContract]
        [WebGet(UriTemplate = "/sessions/whoami", ResponseFormat = WebMessageFormat.Json)]
        Session GetSessionWhoami();

        [OperationContract]
        [WebGet(UriTemplate = "/sessions/me", ResponseFormat = WebMessageFormat.Json)]
        List<Session> GetSessionMe();

        [OperationContract]
        [WebInvoke(Method = "DELETE", UriTemplate = "/sessions/{id}", ResponseFormat = WebMessageFormat.Json)]
        void DeleteSession(string id);

        // # Posts

        [OperationContract]
        [WebGet(UriTemplate = "/posts", ResponseFormat = WebMessageFormat.Json)]
        List<Post> GetPosts();

        [OperationContract]
        [WebGet(UriTemplate = "/posts/{id}", ResponseFormat = WebMessageFormat.Json)]
        Post GetPost(string id);

        [OperationContract]
        [WebInvoke(Method = "POST", UriTemplate = "/posts", ResponseFormat = WebMessageFormat.Json)]
        void InsertPost(Stream stream);

        [OperationContract]
        [WebInvoke(Method = "DELETE", UriTemplate = "/posts/{id}", ResponseFormat = WebMessageFormat.Json)]
        void DeletePost(string id);

        // # Post Files

        [OperationContract]
        [WebGet(UriTemplate = "/post-files/{id}", ResponseFormat = WebMessageFormat.Json)]
        Stream GetPostFile(string id);
    }
}

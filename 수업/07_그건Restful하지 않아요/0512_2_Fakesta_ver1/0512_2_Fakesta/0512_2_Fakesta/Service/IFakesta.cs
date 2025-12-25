// IFakesta.cs

using System;
using System.Collections.Generic;
using System.ServiceModel;
using System.ServiceModel.Web;
using _0512_2_Fakesta.Data;

namespace _0512_2_Fakesta.Service
{
    [ServiceContract]
    internal interface IFakesta
    {
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
        void InsertPost(string title, string filename);

        [OperationContract]
        [WebInvoke(Method = "DELETE", UriTemplate = "/posts/{id}")]
        void DeletePost(string id);
    }
}

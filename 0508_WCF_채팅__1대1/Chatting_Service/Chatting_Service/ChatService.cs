using System;
using System.Collections.Generic;
using System.Linq;
using System.ServiceModel;
using System.Text;
using System.Threading.Tasks;

namespace Chatting_Service
{
    internal class ChatService : IChat
    {

        //델리게이트 선언
        public delegate void Chat(string type, string nickname, string msg, DateTime date);

        // 개인용 델리게이트
        private Chat MyChat;

        //전체에게 보낼 정보를 담고 있는 델리게이트
        IChatCallback callback = null;

        #region 1. Join(로그인하기)
        public bool Join(string nickname, DateTime date)
        {
            MyChat = new Chat(UserHandler);
            //2. 사용자에게 보내 줄 채널을 설정한다.
            callback = OperationContext.Current.GetCallbackChannel<IChatCallback>();

            //현재 접속자 정보를 모두에게 전달
            BroadcastMessage("UserEnter", nickname, "", date);
            return true;
        }
        #endregion

        #region 2. Say(메시지 보내기)
        public void Say(string nickname, string msg, DateTime date)
        {
            BroadcastMessage("Receive", nickname, msg, date);
        }
        #endregion

        #region 3. Leave(로그아웃 하기)
        public void Leave(string nickname, DateTime date)
        {
            Chat d = null;
            MyChat -= d;
        }
        #endregion



        private void BroadcastMessage(string msgType, string nickname, string msg, DateTime date)
        {
            MyChat.BeginInvoke(msgType, nickname, msg, date, new AsyncCallback(EndAsync), null);
        }

        private void UserHandler(string nickname, string msg, string msgType, DateTime date)
        {
            try
            {
                //클라이언트에게 보내기
                switch (msgType)
                {
                    
                    case "Receive": callback.Receive(nickname, msg, date); break;
                    case "UserEnter": callback.UserEnter(nickname, date); break;
                }
            }
            catch//에러가 발생했을 경우
            {
                Console.WriteLine("{0} 에러", nickname);
            }
        }

        private void EndAsync(IAsyncResult ar)
        {
            Chat d = null;
            try
            {
                System.Runtime.Remoting.Messaging.AsyncResult asres = (System.Runtime.Remoting.Messaging.AsyncResult)ar;
                d = ((Chat)asres.AsyncDelegate);
                d.EndInvoke(ar);
            }
            catch
            {
            }
        }

    }
}


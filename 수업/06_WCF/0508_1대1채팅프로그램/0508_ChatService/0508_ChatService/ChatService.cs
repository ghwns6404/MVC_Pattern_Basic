using System;
using System.ServiceModel;

namespace Chating
{
    //델리게이트 선언
    public delegate void Chat(string type, string nickname, string msg, DateTime date);

    internal class ChatService : IChat
    {        
        // 개인용 델리게이트
        private Chat MyChat;

        //전체에게 보낼 정보를 담고 있는 델리게이트
        private IChatCallback callback = null;

        public bool Join(string nickname, DateTime date)
        {
            MyChat = new Chat(UserHandler);
            //2. 사용자에게 보내 줄 채널을 설정한다.
            callback = OperationContext.Current.GetCallbackChannel<IChatCallback>();

            //현재 접속자 정보를 모두에게 전달
            BroadcastMessage("UserEnter", nickname, "", date);
            return true;
        }

        public void Leave(string nickname, DateTime date)
        {
            Chat d = null;
            MyChat -= d;
        }

        public void Say(string nickname, string msg, DateTime date)
        {
            BroadcastMessage("Receive", nickname, msg, date);
        }

        private void BroadcastMessage(string msgType, string nickname, string msg, DateTime date)
        {
            MyChat.BeginInvoke(msgType, nickname, msg, date, new AsyncCallback(EndAsync), null);
        }

        private void UserHandler(string msgType, string nickname, string msg, DateTime date)
        {
            try
            {
                //클라이언트에게 보내기
                switch (msgType)
                {
                    case "Receive":     callback.Receive(nickname, msg, date);  break;
                    case "UserEnter":   callback.UserEnter(nickname, date);     break;
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

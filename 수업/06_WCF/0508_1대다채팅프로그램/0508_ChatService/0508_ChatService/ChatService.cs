using System;
using System.Collections.Generic;
using System.ServiceModel;

namespace Chating
{
    public delegate void Chat(string type, string nickname, string msg, DateTime date);

    class ChatService : IChat
    {
        //동기화 작업을 위해서 가상의 객체 생성
        private static Object syncObj = new Object();

        //채팅방에 있는 유저 이름 목록
        private static List<string> Chatter = new List<string>();

        // 개인용 델리게이트
        private Chat MyChat;

        //전체에게 보낼 정보를 담고 있는 델리게이트 ????
        private static Chat List;
        IChatCallback callback = null;

        //==========================  IChat 인터페이스 
        #region 1. Join(로그인하기)
        public bool Join(string nickname, DateTime date)
        {
            MyChat = new Chat(UserHandler);
            lock (syncObj)
            {

                if (!Chatter.Contains(nickname)) //이름이 기존 채터에 있는지 검색한다.
                {
                    Chatter.Add(nickname);

                    //2. 사용자에게 보내 줄 채널을 설정한다.
                    callback = OperationContext.Current.GetCallbackChannel<IChatCallback>();

                    //델리게이터 추가(향후 데이터 수신이 가능하도록 구성)
                    List += MyChat;

                    //현재 접속자 정보를 모두에게 전달
                    BroadcastMessage("UserEnter", nickname, "", date);                   

                    return true;
                }
                return false;
            }
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
            Chatter.Remove(nickname);

            //메시지 수신에서 제외
            List -= MyChat;

            //모든 사람에게 전송
            string msg = string.Format(nickname + "이가 나갔습니다");
            BroadcastMessage("UserLeave", nickname, msg, date);
        }
        #endregion
        //===================================================================================================        

        private void BroadcastMessage(string msgType, string nickname, string msg, DateTime date)
        {
            if (List != null)
            {
                //현재 이벤트들을 전달한다.
                foreach (Chat handler in List.GetInvocationList())
                {
                    handler.BeginInvoke(msgType, nickname, msg, date, new AsyncCallback(EndAsync), null);
                }
            }
        }

        //클라이언트에게 보내기
        private void UserHandler(string msgType, string nickanme, string msg, DateTime date)
        {
            try
            {
                switch (msgType)
                {
                    case "UserEnter":   callback.UserEnter(nickanme, date); break;
                    case "Receive":     callback.Receive(nickanme, msg, date); break;
                    case "UserLeave":   callback.UserLeave(nickanme, date); break;
                }
            }
            catch//에러가 발생했을 경우
            {
                Leave(nickanme, date);
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
                List -= d;
            }
        }

    }
}

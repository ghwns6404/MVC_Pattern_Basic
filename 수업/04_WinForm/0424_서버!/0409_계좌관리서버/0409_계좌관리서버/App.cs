//App.cs
using WoosongBit41.Lib;

namespace _0409_계좌관리서버
{
    internal class App
    {
        #region 0. 싱글톤 패턴
        public static App Singleton { get; } = null;
        static App() { Singleton = new App(); }
        private App() { }
        #endregion

        public void Init()
        {
            AccountControl.Singleton.Init();
            WbLib.Logo();
        }

        public void Exit()
        {
            AccountControl.Singleton.Exit();
            WbLib.Ending();
        }
    }
}

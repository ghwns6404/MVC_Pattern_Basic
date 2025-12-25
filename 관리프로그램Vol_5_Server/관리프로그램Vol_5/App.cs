//App.cs

namespace 관리프로그램Vol_5
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
            DataControl.Singleton.Init();
            WbLib.Logo();
        }

        public void Exit()
        {
            DataControl.Singleton.Exit();
            WbLib.Ending();
        }
    }
}

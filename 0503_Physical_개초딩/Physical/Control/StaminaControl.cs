//Control.cs

namespace Physical
{
    public class StaminaControl
    {
        #region 싱글톤
        public static StaminaControl Singleton { get; } = null;
        static StaminaControl() { Singleton = new StaminaControl(); }
        public StaminaControl() { }
        #endregion

        public Human human = new Human(30);

        public void Control_Walk()
        {
            human.Walk();
            
        }
        public void Control_Run()
        {
            human.Run();
        }
        public void Control_Sleep()
        {
            human.Sleep();
        }

    }
}

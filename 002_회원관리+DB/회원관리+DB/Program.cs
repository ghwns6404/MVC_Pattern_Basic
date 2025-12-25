//Program.cs 




namespace 회원관리_DB
{
    internal class Program
    {
        static void Main(string[] args)
        {
            App app = App.Singleton;
            app.Init();
            app.Run();
            app.Exit();
        }
    }
}

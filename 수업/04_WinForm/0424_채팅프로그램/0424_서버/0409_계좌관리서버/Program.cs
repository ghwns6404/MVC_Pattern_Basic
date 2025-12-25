//Program.cs

using System;
using WoosongBit41.Lib;

namespace _0409_계좌관리서버
{
    internal class Program
    {
        static void Main(string[] args)
        {
            App app = App.Singleton;
            app.Init();
            Console.Clear();
            Console.ReadLine();
            app.Exit();
        }
    }
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace _0604_test
{
    internal class Program
    {
        static void Main(string[] args)
        {
            App app = new App();
            app.Init();
            app.Run();
            app.Exit();
        }
    }
}

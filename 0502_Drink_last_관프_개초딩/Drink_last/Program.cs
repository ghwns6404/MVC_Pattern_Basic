using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Remoting.Messaging;
using System.Text;
using System.Threading.Tasks;

namespace Drink_last
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

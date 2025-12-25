using System;
using System.Collections.Generic;
using System.Linq;
using System.Linq.Expressions;
using System.Text;
using System.Threading.Tasks;

namespace _0604_test
{
    public class App
    {
        public WbLib lib = new WbLib();
        public Control control = new Control();

        public void Init()
        {
            lib.Logo();
        }

        public void Run()
        {
            while(true)
            {
                Console.Clear();

                switch (lib.Menu())
                {
                    case ConsoleKey.Escape : return;
                    case ConsoleKey.F1: control.Insert(); break;
                    case ConsoleKey.F2: control.Delete(); break;
                    case ConsoleKey.F3: control.Select(); break;
                    case ConsoleKey.F4: control.Update(); break;
                    case ConsoleKey.F5: control.PrintAll(); break;
                    default: Console.WriteLine("그런 기능은 없다"); break;
                }

            }
        }

        public void Exit()
        {
            lib.Ending();
        }
    }
}

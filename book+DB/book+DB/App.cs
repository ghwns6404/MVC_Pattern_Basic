using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace book_DB
{
    public class App
    {
        public WbLib lib = new WbLib();
        public Control con = new Control();

        public void Init()
        {
            con.Connect();
            lib.Logo();
        }
        public void Run()
        {
            while(true)
            {
                Console.Clear();
                switch (lib.Menu())
                {
                    case ConsoleKey.Escape: return;
                    case ConsoleKey.F1: con.Con_Insert(); break;
                    case ConsoleKey.F2: con.Con_Select(); break;
                    case ConsoleKey.F3: con.Con_Update(); break;
                    case ConsoleKey.F4: con.Con_Delete(); break;
                    case ConsoleKey.F5: con.Con_PrintAll(); break;
                }
                lib.Pause();
            }

        }
        public void Exit()
        {
            lib.Ending();
            con.Close();
        }

    }
}

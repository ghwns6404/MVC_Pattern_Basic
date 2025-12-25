using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;
//===========================관리프로그램 Vol.5클라 ============
namespace _관리프로그램Vol_5
{
    internal static class Program
    {
        [STAThread]
        static void Main()      //클라
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new Form1());
        }
    }
}

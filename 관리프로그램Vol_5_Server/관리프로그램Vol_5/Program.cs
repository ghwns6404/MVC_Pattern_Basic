using System;
using System.Windows.Forms;
//===========================관리프로그램 Vol. " 5서버 " ===============================================
namespace _관리프로그램Vol_5
{
    internal static class Program
    {
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Console.WriteLine("관리프로그램 Vol.5 서버 !");
            Application.Run(new Form1());
        }
    }
}

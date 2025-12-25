using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Drawing;
using System.Security.Cryptography;


namespace _0422_WinFrorm
{
    internal class Program
    {
         
        static void Sample3()
        {
            string[] strText = { "빨", "주", "노", "초", "파", "남", "보" };
            Form[] wnd = new Form[7];
            for (int i = 0; i < 7; i++)
            {
                wnd[i] = new Form();
                wnd[i].Text = strText[i];
                wnd[i].SetBounds((i + 1) * 10, (i + 1) * 10, 100, 100);
                wnd[i].MaximizeBox = false;
                wnd[i].Show();
                Console.WriteLine("{0} 번째 윈도우 출력 성공!!!", i);
            }
        }
        static void Sample2()
        {
            Form form = new Form();
            form.Text = "첫번째 윈도우";         //제목에 출력
            form.SetBounds(100, 100, 500, 500); //윈폼 크기 
            form.MaximizeBox = false;           //MAX창 크기 조절 허용여부
            form.MinimizeBox = false;           //MIN창 크기 조절 허용여부
            form.StartPosition = FormStartPosition.CenterScreen; //출력시 화면 딱 중간에 출력
            form.Opacity = 1;                 //창의 투명도 설정

            Application.Run(form);
        }
        static void Sample1()
        {
            Application.Run(new Form());
        }

        static void Main(string[] args)
        {

            Sample3();
        }
    }
}

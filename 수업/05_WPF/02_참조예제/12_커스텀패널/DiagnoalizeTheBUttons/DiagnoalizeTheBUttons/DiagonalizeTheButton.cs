using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;

namespace DiagnoalizeTheBUttons
{
    class DiagonalizeTheButton:Window
    {
        [STAThread]
        public static void Main()
        {
            Application app = new Application();
            app.Run(new DiagonalizeTheButton());
        }

        public DiagonalizeTheButton()
        {
            Title = "Diagonialize the Buttons";

            //DiagonalPanel 패널 생성
            DiagonalPanel pnl = new DiagonalPanel();
            Content = pnl;
            //배경 블랙
            pnl.Background = Brushes.Black;
            //랜덤 객체 생성
            Random rand = new Random();

            //다양한 크기를 가진 5개의 버튼을 배치
            for (int i = 0; i < 5; i++)
            {
                Button btn = new Button();
                btn.Content = "Button Number"+(i+1);
                btn.FontSize += rand.Next(20);
                pnl.Add(btn);
            }
        }
    }
}

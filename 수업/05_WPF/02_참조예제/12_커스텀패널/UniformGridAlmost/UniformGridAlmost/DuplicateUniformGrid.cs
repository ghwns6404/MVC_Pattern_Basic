using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Media;
using System.Windows.Input;
using System.Windows.Controls;

namespace UniformGridAlmost
{
    class DuplicateUniformGrid:Window
    {
        [STAThread]
        public static void Main()
        {
            Application app = new Application();
            app.Run(new DuplicateUniformGrid());
        }

        public DuplicateUniformGrid()
        {
            Title = "Duplicate Uniform Grid";            

            //UniformGridAlmost 객체 생성
            UniformGridAlmost unigrid = new UniformGridAlmost();
            unigrid.Columns = 5;
            Content = unigrid;
            
            //랜덤 객체 생성 UniformGridAlmost에 임의의 크기를 가진 버튼을 채움
            Random rand = new Random();

            for (int index = 0; index < 48; index++)
            {
                Button btn = new Button();
                btn.Name = "Buttton"+index;
                btn.Content = btn.Name;
                //폰트사이즈를 랜덤으로...
                //주석을 하게되면 ? ? ?
                btn.FontSize += rand.Next(10);
                unigrid.Children.Add(btn);
            }
            AddHandler(Button.ClickEvent,new RoutedEventHandler(ButtonOnClick));
            }
        void ButtonOnClick(object sender, RoutedEventArgs args)
        {
            Button btn = args.Source as Button;
            MessageBox.Show(btn.Name+"has been clicked",Title);
        }
    }
}

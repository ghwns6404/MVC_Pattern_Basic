//--------------------------------------------------
// SplitNine.cs (c) 2006 by Charles Petzold
//--------------------------------------------------
using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;

namespace Petzold.SplitNine
{
    public class SplitNine : Window
    {
        [STAThread]
        public static void Main()
        {
            Application app = new Application();
            app.Run(new SplitNine());
        }

        public SplitNine()
        {
            Title = "Split Nine";

            Grid grid = new Grid();
            Content = grid;

            // 그리드 3×3 행렬을 만든다.
            for (int i = 0; i < 3; i++)
            {
                grid.ColumnDefinitions.Add(new ColumnDefinition()); // 열 추가
                grid.RowDefinitions.Add(new RowDefinition());       //행 추가
            }

            // 9개의 버튼을 만든다.
            for (int x = 0; x < 3; x++)
                for (int y = 0; y < 3; y++)
                {
                    Button btn = new Button();
                    btn.Content = "Row " + y + " and Column " + x;
                    //btn.Margin = new Thickness(10);  // 간격 추가
                    grid.Children.Add(btn);
                    Grid.SetRow(btn, y);  //행의 추가 
                    Grid.SetColumn(btn, x); // 열의 추가.
                }
           
            ////경계를 만든다.
            //for (int i = 0; i < 3; i++)
            //{
            //    for (int j = 0; j < 3; j++)
            //    {
            //        GridSplitter split = new GridSplitter();
            //        split.Width = 20;
            //        grid.Children.Add(split);
            //        Grid.SetRow(split, i);
            //        Grid.SetColumn(split, j);
            //    }
            //}

            GridSplitter split = new GridSplitter();
     
            //split.HorizontalAlignment = HorizontalAlignment.Center; //구간설정

            //split.HorizontalAlignment = HorizontalAlignment.Stretch;  //위에 퍼지게
            //split.VerticalAlignment = VerticalAlignment.Top;
            //split.Height = 30;

            //split.ResizeDirection = GridResizeDirection.Columns;  // 움직이게 가능하게 하기

            grid.Children.Add(split);
            Grid.SetRow(split, 0);
            Grid.SetColumn(split, 1);
            Grid.SetRowSpan(split, 3);

        }
    }
}
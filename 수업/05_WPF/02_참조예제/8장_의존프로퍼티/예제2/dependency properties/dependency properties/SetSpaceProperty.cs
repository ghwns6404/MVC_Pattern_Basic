//-------------------------------------------------
// SetSpaceProperty.cs (c) 2006 by Charles Petzold
//-------------------------------------------------
using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;

namespace Petzold.SetSpaceProperty
{
    public class SetSpaceProperty : SpaceWindow
    {
        [STAThread]
        public static void Main()
        {
            Application app = new Application();
            app.Run(new SetSpaceProperty());
        }


        public SetSpaceProperty()
        {
            
            Title = "글자에 빈 공간 넣기";
            SizeToContent = SizeToContent.WidthAndHeight;
           // ResizeMode = ResizeMode.CanMinimize;

            //글자사이에 넣을 공백 값 배열 저장
            int[] iSpaces = { 0, 1, 2 };

            // 그리드 만들고..
            Grid grid = new Grid();
            Content = grid;
            //셋팅
            for (int i = 0; i < 2; i++)                  // 행 2개
            {
                RowDefinition row = new RowDefinition();
                row.Height = GridLength.Auto;
                grid.RowDefinitions.Add(row);
            }
            for (int i = 0; i < iSpaces.Length; i++) // 열 3개
            {
                ColumnDefinition col = new ColumnDefinition();
                col.Width = GridLength.Auto;
                grid.ColumnDefinitions.Add(col);
            }


            for (int i = 0; i < iSpaces.Length; i++)
            {
                SpaceButton btn = new SpaceButton();
                btn.Text = "글자사이에 공간이 늘어 나네요:" + iSpaces[i];

                //사용자 지정 정보를 저장하는데 사용할 수 있는 임의의 개체값 가져옮
                btn.Tag = iSpaces[i];

                btn.HorizontalAlignment = HorizontalAlignment.Center;
                btn.VerticalAlignment = VerticalAlignment.Center;

                // 클릭 이벤트 핸들러 등록
                btn.Click += WindowPropertyOnClick;

                // 그리드에 버튼을 그림
                grid.Children.Add(btn);
                Grid.SetRow(btn, 0);
                Grid.SetColumn(btn, i);

                btn = new SpaceButton();
                btn.Text = "글자사이에 공간이 늘어 나네요 : " + iSpaces[i];
                btn.Tag = iSpaces[i];
                btn.HorizontalAlignment = HorizontalAlignment.Center;
                btn.VerticalAlignment = VerticalAlignment.Center;
                btn.Click += ButtonPropertyOnClick;
                grid.Children.Add(btn);
                Grid.SetRow(btn, 1);
                Grid.SetColumn(btn, i);
            }
        }

        // 버튼클릭했을때발생하는이벤트- 위에버튼
        void WindowPropertyOnClick(object sender, RoutedEventArgs args)
        {
            SpaceButton btn = args.Source as SpaceButton;
            Space = (int)btn.Tag;
        }
        // 버튼클릭했을때발생하는이벤트- 아래버튼
        void ButtonPropertyOnClick(object sender, RoutedEventArgs args)
        {
            SpaceButton btn = args.Source as SpaceButton;
            btn.Space = (int)btn.Tag;
        }
    }
}

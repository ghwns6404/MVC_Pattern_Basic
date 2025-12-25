using System;
using System.Reflection;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;
using System.Globalization; // 날짜

namespace Petzold.ListColorNames
{
    class ListColorNames : Window
    {
        [STAThread]
        public static void Main()
        {
            Application app = new Application();
            app.Run(new ListColorNames());
        }
        public ListColorNames()
        {
            Title = "List Color Names";
            
            ListBox lstbox = new ListBox();
            lstbox.Width = 150;   // 폭
            lstbox.Height = 150;  //높이 
            lstbox.SelectionChanged += ListBoxOnSelectionChanged;   // 이벤트 등록 
            Content = lstbox;          //win에 등록(화면에 출력)
           
           // lstbox.Items.Add("월요일");
           // lstbox.Items.Add("화요일");
            //lstbox.Items.Add("수요일");
            //lstbox.Items.Add("목요일");
           
            //lstbox.ItemsSource = DateTimeFormatInfo.CurrentInfo.DayNames;   //자국어
            //lstbox.ItemsSource = DateTimeFormatInfo.InvariantInfo.DayNames; //영어
           
            //1번
            /*
            //색상명으로 ListBox를 체운다
            PropertyInfo[] props = typeof(Colors).GetProperties();
            //GetProperties 로 Colors에 있는 프로퍼티 141개를 배열로 가져온다.
            //가져온 PropertyInfo 타입의 배열을 하나씩 가져와 Name으로 이름을 리스트 박스에 추가한다.
            foreach (PropertyInfo prop in props)
                lstbox.Items.Add(prop.Name);
            */


            //2번
            /*
            PropertyInfo[] props = typeof(Colors).GetProperties();
            //리스트 박스를 Color 값으로 체운다...
            foreach (PropertyInfo prop in props)
                lstbox.Items.Add(prop.GetValue(null, null));
           */

          
        }

        //리스트의 선책항목이 변했을떄 발생하는 이벤트 
        void ListBoxOnSelectionChanged(object sender, SelectionChangedEventArgs args)
        {
            //파라미터로 넘어온 자기 자신을 형변환 하여 담는다. 
            ListBox lstbox = sender as ListBox;


            //1번
            /*
            //가져온 객체의 선택된 Item 을 string 형태로 캐스팅 하여 string에 담는다.
            string str = lstbox.SelectedItem as string;
            //Selectedindex 범위는 0 부터 아이탬 개수 -1 까지가 된다.
            //변환된 string값이 null이 아니면 배경색을 바꾼다.
            if (str != null)
            {
                //실질적인 이름으로 Color의 값을 반환 받는방법 
                Color clr = (Color)typeof(Colors).GetProperty(str).GetValue(null, null);

                Background = new SolidColorBrush(clr);
            }
            */

            //2번
            /*
            if (lstbox.SelectedIndex != -1)
            {
                Color clr = (Color)lstbox.SelectedItem;
                Background = new SolidColorBrush(clr);
            }
            */
            
        }
            



    }
}

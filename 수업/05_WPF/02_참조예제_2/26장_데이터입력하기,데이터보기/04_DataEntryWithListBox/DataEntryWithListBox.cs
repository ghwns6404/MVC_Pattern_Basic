//-----------------------------------------------------
// DataEntryWithListBox.cs (c) 2006 by Charles Petzold
//-----------------------------------------------------
using Petzold.MultiRecordDataEntry;
using Petzold.SingleRecordDataEntry;
using System;
using System.Collections.Specialized;
using System.ComponentModel;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Input;
using System.Windows.Media;

namespace Petzold.DataEntryWithListBox
{
    public partial class DataEntryWithListBox
    {
        ListCollectionView collview;
        People people;

        [STAThread]
        public static void Main()
        {
            Application app = new Application();
            app.Run(new DataEntryWithListBox());
        }
        public DataEntryWithListBox()
        {
            InitializeComponent();

            // File New 명령을 시뮬레이션
            ApplicationCommands.New.Execute(null, this);

            // 패널의 첫번째 텍스트박스에 포커스를 준다.
            pnlPerson.Children[1].Focus();
        }

        // 새로하기 이벤트 핸들러
        void NewOnExecuted(object sender, ExecutedRoutedEventArgs args)
        {
            people = new People();
            people.Add(new Person());
            InitializeNewPeopleObject();
        }

        // 데이터열기 이벤트 핸들러 (파일 IO부분) 
        void OpenOnExecuted(object sender, ExecutedRoutedEventArgs args)
        {
            people = People.Load(this);

            if (people != null)
                InitializeNewPeopleObject();
        }
        // 데이터 저장 메뉴 이벤트 핸들러 (파일 IO부분)
        void SaveOnExecuted(object sender, ExecutedRoutedEventArgs args)
        {
            people.Save(this);
        }

        void InitializeNewPeopleObject()
        {
            // People객체를 기반으로 ListCollectionView 생성 (포인트!!)
            collview = new ListCollectionView(people);

            // 뷰(View)를 정렬하기 위해 프로퍼티를 설정
            // "LastName"을 기준으로 비교 한뒤 오름차순으로 정렬
            collview.SortDescriptions.Add(
                new SortDescription("LastName", ListSortDirection.Ascending));

            // ListCollectionView를 통해 ListBox와 PersonPanel을 연결
            lstbox.ItemsSource = collview;

            // ListBox를 이용해 컬렉션을 탐색하기 위해 설정
            pnlPerson.DataContext = collview;

            // 리스트박스의 SelectedIndex 설정하기
            if (lstbox.Items.Count > 0)                     // 리스트박스의 아이템이 있으면 ' ㅁ'
                lstbox.SelectedIndex = 0;                  // 맨 위에꺼 선택되어 있게
        }

        // 데이터 추가 버튼 클릭 이벤트 핸들러
        void AddOnClick(object sender, RoutedEventArgs args)
        {
            Person person = new Person();
            people.Add(person);
            lstbox.SelectedItem = person;
            pnlPerson.Children[1].Focus();

            // 리스트뷰 갱신
            collview.Refresh();
        }

        void DeleteOnClick(object sender, RoutedEventArgs args)
        {
            if (lstbox.SelectedItem != null)
            {
                people.Remove(lstbox.SelectedItem as Person);

                if (lstbox.Items.Count > 0)
                    lstbox.SelectedIndex = 0;
                else
                {
                    // 무조건 빈 데이터라도 있어야... 라고 해놨네여 > ㅁ<
                    AddOnClick(sender, args);
                }
            }
        }
    }
}

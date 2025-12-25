//--------------------------------------------------------
// DataEntryWithNavigation.cs (c) 2006 by Charles Petzold
//--------------------------------------------------------
using Petzold.DataEntry;
using Petzold.MultiRecordDataEntry;
using Petzold.SingleRecordDataEntry;
using System;
using System.Collections.Specialized;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;

namespace Petzold.DataEntryWithNavigation
{
    public partial class DataEntryWithNavigation
    {
        People people;

        [STAThread]
        public static void Main()
        {
            Application app = new Application();
            app.Run(new DataEntryWithNavigation());
        }

        public DataEntryWithNavigation()
        {
            InitializeComponent();

            // File New 명령을 시뮬레이션
            ApplicationCommands.New.Execute(null, this);

            // 패널의 첫번째 텍스트박스에 포커스를 준다.
            pnlPerson.Children[1].Focus();
        }

        // 메뉴 항목을 위한 이벤트 핸들러
        void NewOnExecuted(object sender, ExecutedRoutedEventArgs args)
        {
            people = new People();
            people.Add(new Person());
            InitializeNewPeopleObject();
        }
        void OpenOnExecuted(object sender, ExecutedRoutedEventArgs args)
        {
            people = People.Load(this);
            InitializeNewPeopleObject();
        }
        void SaveOnExecuted(object sender, ExecutedRoutedEventArgs args)
        {
            people.Save(this);
        }

        void InitializeNewPeopleObject()
        {
            // NavigationBar의 Collection 프로퍼티를 새로 생성한 people객체로 설정
            navbar.Collection = people;

            // 네비게이션바의 ItemType 프로퍼티 설정
            navbar.ItemType = typeof(Person);

            // PersonPanel의 DataContext를 people객체로 설정
            // 이거때문에 패널이 Person클래스의 모든 프로퍼티 표시 가능
            pnlPerson.DataContext = people;
        }
    }
}

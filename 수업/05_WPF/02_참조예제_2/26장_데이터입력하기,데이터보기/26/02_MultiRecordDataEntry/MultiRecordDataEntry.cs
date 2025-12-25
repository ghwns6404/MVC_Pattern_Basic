//-----------------------------------------------------
// MultiRecordDataEntry.cs (c) 2006 by Charles Petzold
//-----------------------------------------------------
using Petzold.SingleRecordDataEntry;
using System;
using System.Collections.Specialized;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;

namespace Petzold.MultiRecordDataEntry
{
    public partial class MultiRecordDataEntry
    {
        People people;                                   // People 클래스 객체 선언
        int index;                                          // Person 컬렉션 객체의 인덱스 저장 변수 선언

        [STAThread]
        public static void Main()
        {
            Application app = new Application();
            app.Run(new MultiRecordDataEntry());
        }

        public MultiRecordDataEntry()
        {
            InitializeComponent();

            // File New 명령을 시뮬레이션
            ApplicationCommands.New.Execute(null, this);

            // 패널의 첫번째 텍스트박스에 포커스를 준다.
            // pnlPerson이라는 이름은 xaml코드에서 설정해 줬음
            pnlPerson.Children[1].Focus();
        }

        // New메뉴 클릭
        void NewOnExecuted(object sender, ExecutedRoutedEventArgs args)
        {
            people = new People();                          // People 클래스 객체 생성
            InitializeNewPeopleObject();                    // 초기화 매서드 호출 (People.cs)
        }

        // Open메뉴 클릭
        void OpenOnExecuted(object sender, ExecutedRoutedEventArgs args)
        {
            people = People.Load(this);                     // Load 메서드 에서 반환된 객체 저장       
            InitializeNewPeopleObject();                    // 초기화 매서드 호출 (People.cs)
        }

        // Save메뉴 클릭
        void SaveOnExecuted(object sender, ExecutedRoutedEventArgs args)
        {
            people.Save(this);                                  // Save 메서드 호출 (People.cs)
        }

        // 초기화
        void InitializeNewPeopleObject()
        {
            index = 0;

            if (people.Count == 0)                           // 컬렉션이 없으면 ??
                people.Insert(0, new Person());             // 0번째 인덱스에 새로운 Person객체 생성

            pnlPerson.DataContext = people[0];          // DataContext에 people의 0번째 배열에 있는 객체를 넣음
            EnableAndDisableButtons();                     // 버튼 활성화 관련 메서드          
        }

        // 처음으로 버튼 클릭 이벤트 핸들러
        void FirstOnClick(object sender, RoutedEventArgs args)
        {
            // DataContext에 인덱스를 0으로 초기화 하며
            // people의 0번째 인덱스의 객체를 넣어준다. (맨 처음객체로 이동)
            pnlPerson.DataContext = people[index = 0];  
            EnableAndDisableButtons();
        }

        // 이전 버튼 클릭 이벤트 핸들러
        void PrevOnClick(object sender, RoutedEventArgs args)
        {
            // DataContext에 현재 인덱스보다 하나 작은(이전배열) 인덱스의 객체를 넣는다.
            pnlPerson.DataContext = people[index -= 1];
            EnableAndDisableButtons();
        }

        // 다음 버튼 클릭 이벤트 핸들러
        void NextOnClick(object sender, RoutedEventArgs args)
        {
            pnlPerson.DataContext = people[index += 1];
            EnableAndDisableButtons();
        }

        // 마지막 버튼 클릭 이벤트 핸들러
        void LastOnClick(object sender, RoutedEventArgs args)
        {
            // DataContext에 마지막 인덱스 객체를 넣어준다.
            // 배열은 0부터 시작이므로 최종 Count에서 -1을 해주어야 한다.
            pnlPerson.DataContext = people[index = people.Count - 1];
            EnableAndDisableButtons();
        }

        // 데이터 추가 버튼 클릭 이벤트 핸들러
        void AddOnClick(object sender, RoutedEventArgs args)
        {
            people.Insert(index = people.Count, new Person());
            pnlPerson.DataContext = people[index];
            EnableAndDisableButtons();
        }

        // 데이터 삭제 버튼 클릭 이벤트 핸들러
        void DelOnClick(object sender, RoutedEventArgs args)
        {
            people.RemoveAt(index);                      // 지정한 인덱스에서 요소 제거

            if (people.Count == 0)                         // 만약 인덱스가 0이라면?
                people.Insert(0, new Person());           // 새로운 0번째 인덱스 객체 생성

            if (index > people.Count - 1)                 // 만약 인덱스가 마지막 이었다면
                index--;                                        // 지우고 나서 인덱스 번호 -1을 해줌
            
            pnlPerson.DataContext = people[index];   //DataContext에 현재 인덱스 번호의 객체를 넣음 
            EnableAndDisableButtons();
        }

        // 버튼 활성화 관련
        void EnableAndDisableButtons()
        {
            // 현재표시되고 있는 레코드가 처음이 아니라면 '이전' 버튼 활성화
            btnPrev.IsEnabled = index != 0;

            // 현재 표시되고 있는 레코드가 마지막 레코드가 아니면 '다음' 버튼 활성화
            btnNext.IsEnabled = index < people.Count - 1;

            // 패널의 첫번째 텍스트박스에 포커스를 준다.
            pnlPerson.Children[1].Focus();
        }
    }
}

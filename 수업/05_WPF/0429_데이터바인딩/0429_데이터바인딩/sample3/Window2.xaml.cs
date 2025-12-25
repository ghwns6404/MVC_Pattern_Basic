using System;
using System.ComponentModel;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
/*
[예제 5-21]
List Control(WPF) ----->{cur item 변경}  --> UI갱신
 <ListBox ItemsSource="{Binding}" IsSynchronizedWithCurrentItem="True"/>

List<Person> --->(컬렉션 뷰) -->{cur item } --> UI갱신 
ICollectionView view = CollectionViewSource.GetDefaultView(FindResource("people"));
Person person = (Person)view.CurrentItem;

ICollectionView view = CollectionViewSource.GetDefaultView(FindResource("people"));
view.MoveCurrentToNext();

[예제 5-23] DataTemplate : List에 출력되는 데이터형식을 정의
*/



namespace _0429_데이터바인딩
{
    /// <summary>
    /// Window2.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class Window2 : Window
    {
        private People people = null;
        public Window2()
        {
            InitializeComponent();

            //XMAL어딘가 people이라는 이름을 찾아내서 people에 저장
            people = (People)FindResource("people");

            //에러 핸들러 등록
            //즉 유효성 검사 에러 핸들러들인데 
            //유효성 검사에서 에러가 발생하면 이 핸들러가 호출됨
            Validation.AddErrorHandler(age, age_ValidationError); //이걸로 이벤트를 구독
            Validation.AddErrorHandler(male, age_ValidationError);
        }

        private void age_ValidationError(object sender, ValidationErrorEventArgs e)
        {                             //sender: 누가 이벤트를 발생시켰는지, 예들들어 age Textbox
                                      //Valid-- e : 무슨 에러인지의 정보가 담겨있음
            MessageBox.Show((string)e.Error.ErrorContent);
            //age.ToolTip = (string)e.Error.ErrorContent;
        }


        private void prev_Click(object sender, RoutedEventArgs e)
        {
            //people이라는 리소스(People객체)를 '컬렉션 뷰' 형태로 가져옴 
            ICollectionView view = CollectionViewSource.GetDefaultView(FindResource("people"));
            view.MoveCurrentToPrevious();
            if (view.IsCurrentBeforeFirst)
            {//예외처리 부분이니까 니가 잘 생각해봐
                view.MoveCurrentToFirst();
            }
        }

        private void next_Click(object sender, RoutedEventArgs e)
        {//prev_Click이 이해됐다면 당연히 이해가 될 코드
            ICollectionView view = CollectionViewSource.GetDefaultView(FindResource("people"));
            view.MoveCurrentToNext();
            if (view.IsCurrentAfterLast)
            {
                view.MoveCurrentToLast();
            }
        }
        private void eraseButton_Click(object sender, RoutedEventArgs e)
        {//똑같음
            ICollectionView view = CollectionViewSource.GetDefaultView(FindResource("people"));
            Person person = (Person)view.CurrentItem;
            person.Name = "";
            person.Phone = "";
            person.Age = null;
            person.Male = null;
        }

        private void removeButton_Click(object sender, RoutedEventArgs e)
        {
            
            People people = (People)FindResource("people");
            ICollectionView view = CollectionViewSource.GetDefaultView(FindResource("people"));
            Person person = (Person)view.CurrentItem;
            people.Remove(person);
           
            /*
            People people = (People)FindResource("people");
            if(listbox.SelectedIndex >= 0)
                people.RemoveAt(listbox.SelectedIndex);            
            */
        }

        private void sortButton_Click(object sender, RoutedEventArgs e)
        {
            ICollectionView view = CollectionViewSource.GetDefaultView(FindResource("people"));
            if (view.SortDescriptions.Count == 0)
            {
                view.SortDescriptions.Add(new SortDescription("Name",ListSortDirection.Ascending));
                view.SortDescriptions.Add(new SortDescription("Male",ListSortDirection.Ascending));
            }
            else
            {
                view.SortDescriptions.Clear();
            }
        }

        private void filterButton_Click(object sender, RoutedEventArgs e)
        {
            ICollectionView view = CollectionViewSource.GetDefaultView(FindResource("people"));
            if (view.Filter == null)
            {
                view.Filter = delegate (object obj)
                {
                    return ((Person)obj).age < int.Parse(sortAge.Text);
                };
            }
            else
            {
                view.Filter = null;
            }
        }

    }
}

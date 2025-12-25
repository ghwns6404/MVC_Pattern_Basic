//MainViewModel.cs
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Runtime.CompilerServices;
using System.Windows.Data;


namespace _0429_바인딩사용실습
{
    internal class MainViewModel : INotifyPropertyChanged
    {
        public ObservableCollection<Book> Books { get; set; } = new ObservableCollection<Book>();

        #region 리스트에서 선택된 Member를 저장하는 속성
        private Book _selectedMember;
        public Book SelectedMember
        {
            get // get => _selectedMember;
            {
                return _selectedMember;
            }
            set
            {
                _selectedMember = value;
                OnPropertyChanged();
            }
        }
        #endregion

        #region 저장, 수정시 사용하는 속성(책이름, 가격, 저자, 이미지, 수량)
        private string _title;
        private string _price;
        private string _publisher;
        private string _image;
        private string _stock;
        public string Title
        {
            get => _title;
            set { _title = value; OnPropertyChanged(); }
        }
        public string Price
        {
            get => _price;
            set { _price = value; OnPropertyChanged(); }
        }
        public string Publisher
        {
            get => _publisher;
            set { _publisher = value; OnPropertyChanged(); }
        }
        
        public string Image
        {
            get => _image;
            set { _image = value; OnPropertyChanged(); }
        }
        public string Stock
        {
            get => _stock;
            set { _stock = value; OnPropertyChanged(); }
        }
        #endregion

        #region 검색(필터)에서 사용되는 컬렉션뷰, 속성(검색이름)
        public ICollectionView FilteredMembers { get; }

        private string _searchText;
        public string SearchText
        {
            get => _searchText;
            set
            {
                _searchText = value;
                OnPropertyChanged();
                FilteredMembers.Refresh();  // 필터 갱신
            }
        }
        #endregion


        #region 속성 변경시 호출되는 통지 이벤트 
        public event PropertyChangedEventHandler PropertyChanged;
        private void OnPropertyChanged([CallerMemberName] string name = null) =>
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(name));
        #endregion


        public MainViewModel()
        {
            

            // 필터링을 위한 View 생성
            FilteredMembers = CollectionViewSource.GetDefaultView(Books);
            FilteredMembers.Filter = FilterMembers;
        }

        #region 기능 
        //자동 호출
        public bool FilterMembers(object obj)
        {
            if (obj is Book book)
            {
                if (string.IsNullOrWhiteSpace(SearchText))
                    return true;
                return book.Title.Contains(SearchText);
            }
            return false;
        }

        #endregion
    }
}

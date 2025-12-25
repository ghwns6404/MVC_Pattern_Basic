//MainViewModel.cs
using System;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Runtime.CompilerServices;
using System.Windows.Data;
using System.Windows.Input;

namespace _0430_MVVM
{
    internal class MainViewModel : INotifyPropertyChanged
    {        
        public ObservableCollection<Member> Members { get; set; } = new ObservableCollection<Member>();

        #region 리스트에서 선택된 Member를 저장하는 속성
        private Member _selectedMember;
        public Member SelectedMember
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

        #region 저장, 수정시 사용하는 속성(이름, 전화번호)
        private string _name;
        public string Name
        {
            get => _name;
            set { _name = value; OnPropertyChanged(); }
        }
        private string _phone;
        public string Phone
        {
            get => _phone;
            set { _phone = value; OnPropertyChanged(); }
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

        #region Action 정의(생성자 초기화)
        public ICommand AddCommand { get; }
        public ICommand UpdateCommand { get; }
        public ICommand DeleteCommand { get; }
        #endregion

        private int _idCounter = 1;

        public MainViewModel()
        {
            AddCommand    = new RelayCommand(AddMember);
            UpdateCommand = new RelayCommand(UpdateMember, () => SelectedMember != null);
            DeleteCommand = new RelayCommand(DeleteMember, () => SelectedMember != null);

            // 필터링을 위한 View 생성
            FilteredMembers = CollectionViewSource.GetDefaultView(Members);
            FilteredMembers.Filter = FilterMembers;
        }

        #region 기능 
        public void AddMember()
        {
            Members.Add(new Member { Id = _idCounter++, Name = Name, Phone = Phone });
        }

        public void UpdateMember()
        {
            if (SelectedMember != null)
            {
                SelectedMember.Name = Name;
                SelectedMember.Phone = Phone;
            }
        }

        public void DeleteMember()
        {
            if (SelectedMember != null)
            {
                Members.Remove(SelectedMember);
                SelectedMember = null;
            }
        }

        //자동 호출
        public bool FilterMembers(object obj)
        {
            if (obj is Member member)
            {
                if (string.IsNullOrWhiteSpace(SearchText))
                    return true;
                return member.Name.Contains(SearchText) || member.Phone.Contains(SearchText);
            }
            return false;
        }

        #endregion
    }
}


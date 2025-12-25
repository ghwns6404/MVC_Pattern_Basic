using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Linq;
using System.Windows.Documents;
using System.Windows.Input;
using System.Xml;
using Newtonsoft.Json;
using System.IO;


namespace WpfApp1
{
    public class MainViewModel : INotifyPropertyChanged
    {
        public ObservableCollection<Book> Books { get; set; } = new ObservableCollection<Book>();

        private Book _selectedBook;
        public Book SelectedBook
        {
            get => _selectedBook;
            set { _selectedBook = value; OnPropertyChanged(nameof(SelectedBook)); }
        }

        public ICommand AddCommand { get; }
        public ICommand DeleteCommand { get; }

        public ICommand SaveCommand { get; }
        public ICommand LoadCommand { get; }

        public MainViewModel()
        {
            AddCommand = new RelayCommand(AddBook);
            DeleteCommand = new RelayCommand(DeleteBook, () => SelectedBook != null);
            //DeleteCommand = new RelayCommand(DeleteBook);

            // 새 커맨드
            SaveCommand = new RelayCommand(() => SaveToFile());
            LoadCommand = new RelayCommand(() => LoadFromFile());
        }

        private void AddBook()
        {
            Books.Add(new Book { Title = "새 책", Author = "저자", PublishedDate = DateTime.Today });
        }

        private void DeleteBook()
        {
            if (SelectedBook != null)
                Books.Remove(SelectedBook);
        }

        public event PropertyChangedEventHandler PropertyChanged;
        protected void OnPropertyChanged(string name) => PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(name));

        #region 파일IO
        public void SaveToFile(string path = "Books.json")  //인자가 있는 함수는 익명의 void함수로 들어서 람다식으로 사용 ,,line34
        {                           //직렬화
            var json = JsonConvert.SerializeObject(Books.ToList(), Newtonsoft.Json.Formatting.Indented);
            File.WriteAllText(path, json);
        }

        public void LoadFromFile(string path = "Books.json")
        {
            if (File.Exists(path))
            {
                var json = File.ReadAllText(path);
                List<Book> loadedBooks = JsonConvert.DeserializeObject<List<Book>>(json);

                Books.Clear();
                foreach (var book in loadedBooks)
                    Books.Add(book);
            }
        }
        #endregion
    }

}

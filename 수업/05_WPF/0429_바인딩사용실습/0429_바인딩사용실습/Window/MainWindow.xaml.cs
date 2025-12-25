using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace _0429_바인딩사용실습
{
    /// <summary>
    /// MainWindow.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class MainWindow : Window
    {
        private Books books = null;

        BookControl conbook = BookControl.Singleton;

        public MainWindow()
        {
            InitializeComponent();

            books = (Books)FindResource("books");
            conbook.Books_=books;
        }
        #region 입력 수정 삭제 정렬
        private void insert_Click(object sender, RoutedEventArgs e)
        {
            InsertWindow win_insert = new InsertWindow();
            win_insert.Show();
        }

        private void update_Click(object sender, RoutedEventArgs e)
        {
            //if (datagrid.SelectedIndex >= 0)
            //{
            //    //UpdateWindwo win_Update = new UpdateWindwo(books[datagrid.SelectedIndex]);
            //    //win_Update.Idx = datagrid.SelectedIndex;
            //    //win_Update.Show();
            //    UpdateWindwo win_Update = new UpdateWindwo(books[datagrid.SelectedIndex]);
            //    win_Update.Show();
            //}
            UpdateWindwo win_Update = new UpdateWindwo(books[datagrid.SelectedIndex]);
            win_Update.Show();
        }

        private void delete_Click(object sender, RoutedEventArgs e)
        {
            //if (listbox.SelectedIndex >= 0)
            //    books.RemoveAt(listbox.SelectedIndex);
            ICollectionView view = CollectionViewSource.GetDefaultView(FindResource("books"));
            Book person = (Book)view.CurrentItem;
            books.Remove(person);
        }

        private void sort_Click(object sender, RoutedEventArgs e)
        {
            ICollectionView view = CollectionViewSource.GetDefaultView(books);
            if (view.SortDescriptions.Count == 0)
            {
                view.SortDescriptions.Add(new SortDescription("Title", ListSortDirection.Ascending));
                view.SortDescriptions.Add(new SortDescription("Price", ListSortDirection.Ascending));
            }
            else
            {
                view.SortDescriptions.Clear();
            }
        }
        #endregion
    }
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace _0429_바인딩사용실습
{
    /// <summary>
    /// UpdateWindwo.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class UpdateWindwo : Window
    {
        private Book book = null;

        private int idx = -1;

        public int Idx
        {
            get {  return idx; }
            set { idx = value; }
        }

        BookControl conbook = BookControl.Singleton;

        public UpdateWindwo(Book _book)
        {
            InitializeComponent();
            book = (Book)FindResource("book");
            book.Title = _book.Title;
            book.Price = _book.Price;
            book.Publisher = _book.Publisher;
            book.Stock = _book.Stock;
        }

        private void updateButton_Click(object sender, RoutedEventArgs e)
        {

            conbook.Update_book(book, Idx);
            this.Close();
        }
    }
}

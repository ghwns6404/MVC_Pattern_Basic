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
    /// InsertWindow.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class InsertWindow : Window
    {
        private Book book = null;

        BookControl conbook = BookControl.Singleton;

        public InsertWindow()
        {
            InitializeComponent();
            book = (Book)FindResource("book");
        }
        private void addButton_Click(object sender, RoutedEventArgs e)
        {
            conbook.Insert_book(book);
            this.Close();
        }
    }
}

using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace _0429_바인딩사용실습
{
    internal class Books : ObservableCollection<Book>
    {
        public Books()
        {
            this.Add(new Book() { Title = "test", Price = 123, Publisher = "tset", Image = "---", Stock = true });
            this.Add(new Book() { Title = "진격의 거인", Price = 10000, Publisher = "미카사", Image = "---", Stock = true });
            this.Add(new Book() { Title = "귀멸의 칼날", Price = 20000, Publisher = "탄지로", Image = "---", Stock = true });
            this.Add(new Book() { Title = "너의 이름은", Price = 30000, Publisher = "미츠하", Image = "---", Stock = false });
        }
    }
}

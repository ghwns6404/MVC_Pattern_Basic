using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Security.Policy;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Controls;
using System.Xml.Linq;

namespace WPF_관리프로그램
{
    internal class MemberList : ObservableCollection<Member>
    {
        public MemberList()
        {
            this.Add(new Member() {Id = 1 , Name = "박춘자", Age = 10, Phone = "010-1233-1233" });
            this.Add(new Member() {Id = 2 , Name = "김춘자", Age = 20, Phone = "010-4444-1233" });
            this.Add(new Member() {Id = 3 , Name = "제갈춘자", Age = 30, Phone = "010-5555-1233" });
            this.Add(new Member() {Id = 4 , Name = "응우엔춘자", Age = 40, Phone = "010-6666-1233" });
        }
    }
}

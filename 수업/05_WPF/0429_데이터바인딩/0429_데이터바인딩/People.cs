using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;

namespace _0429_데이터바인딩
{
    internal class People : ObservableCollection<Person> //List<Person>
    {
        public People()
        {
            this.Add(new Person() { Name = "박춘자",   Phone = "010-1111-1111", Age = 10, Male = true });
            this.Add(new Person() { Name = "응우엔길", Phone = "010-1111-1111", Age = 10, Male = true });
            this.Add(new Person() { Name = "제갈길동", Phone = "010-2222-2222", Age = 20, Male = false });
            this.Add(new Person() { Name = "김복동",   Phone = "010-3333-3333", Age = 30, Male = true });
        }
    }
}

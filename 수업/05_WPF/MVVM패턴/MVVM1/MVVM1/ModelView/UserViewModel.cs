using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MVVM1
{
    internal class UserViewModel
    {
        private User user;

        public UserViewModel()
        {
            user = new User { Name = "홍길동", Email = "홍길동@example.com" };
        }

        public string UserInfo
        {
            get { return $"{user.Name} - {user.Email}"; }
        }
    }
}

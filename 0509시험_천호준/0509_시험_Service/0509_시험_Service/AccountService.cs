using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace _0509_시험_Service
{
    public class AccountService : IAccount
    {
        private MyDB mydb = MyDB.Singleton;

        public bool InsertAccount(int id, string name, int balance)
        {
            return mydb.db_Insert(id, name, balance);
        }

        public Account SelectAccount(int id)
        {
            return mydb.db_SelectAccount(id);
        }

        public void DeleteAccount(int id)
        {
            mydb.db_Delete(id);
        }

        public bool InputMoney(int id, int money)
        {
            return mydb.db_InputMoney(id, money);
        }

        public List<Account> SelectAllAccount()
        {
            return mydb.db_SelectAllAccount();
        }

    }
}

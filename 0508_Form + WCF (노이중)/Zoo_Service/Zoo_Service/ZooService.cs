using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Zoo_Service
{
    public class ZooService : IZoo
    {
        private MyDB mydb = MyDB.Singleton;

        public void Delete(int id)
        {
            mydb.db_Delete(id); //void니까 리턴하지 않아요 ~ 
        }

        public bool Insert(int id, string name, string country, int age)
        {
            return mydb.db_Insert(id, name, country, age);
        }

        public List<Zoo> Select(int id)
        {
            return mydb.db_Select_Id(id);
        }

        public List<Zoo> Select_All()
        {
            return mydb.db_Select_All();
        }

        public bool Update(int id, string name, string country, int age)
        {
            return mydb.db_Update(id, name, country, age);
        }
    }
}

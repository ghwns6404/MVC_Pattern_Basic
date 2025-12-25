using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace book_DB
{
    public class Control
    {
        DB db = new DB();

        public List<Book> book_list = new List<Book>();
        public void Con_Insert()
        {
            Console.Write("책 데이터 입력\n");

            #region 입력칸
            Console.Write("Id :");
            int id = int.Parse(Console.ReadLine());

            Console.Write("제목 : ");
            string title = Console.ReadLine();

            Console.Write("작가 : ");
            string author = Console.ReadLine();

            Console.Write("가격 : ");
            int price = int.Parse(Console.ReadLine());
            #endregion

            bool b = db.DB_Insert(id, title, author, price);
            try
            {
                if (b == false)
                {
                    Console.WriteLine("저장실패");
                }
                else
                {
                    Console.WriteLine($"\n{id},{title},{author},{price} 의 정보가 저장되었습니다");
                }
            }
            catch(Exception ex)
            {
                Console.WriteLine(ex.Message);
            }
        }
        public void Con_Select()
        {
            Console.Write("검색할 ID 입력\n");

            Console.Write("Id :");
            int id = int.Parse(Console.ReadLine());

            string str = db.DB_Select(id);
            string[] strs = str.Split('%');
            try
            {
                int _id = int.Parse(strs[0].ToString());
                string _title = strs[1].ToString();
                string _author = strs[2].ToString();
                int _price = int.Parse(strs[3].ToString());

                Console.WriteLine($"ID{_id}번의 정보는 {_title},{_author},{_price}");
            }
            catch(Exception ex)
            {
                Console.WriteLine(ex.Message);
            }

        }
        public void Con_Update()
        {
            Console.Write("수정할 ID 입력\n");
            #region 입력칸
            Console.Write("Id :");
            int id = int.Parse(Console.ReadLine());

            Console.Write("제목 : ");
            string title = Console.ReadLine();

            Console.Write("작가 : ");
            string author = Console.ReadLine();

            Console.Write("가격 : ");
            int price = int.Parse(Console.ReadLine());
            #endregion

            bool b = db.DB_Update(id, title, author, price);
            try
            {
                if (b == false)
                {
                    Console.WriteLine("수정실패");
                }
                else
                {
                    Console.WriteLine($"\n{id} 의 정보가 {title},{author},{price} 로 수정되었습니다");
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
            }
        }
        public void Con_Delete()
        {
            Console.Write("삭제할 ID 입력\n");

            Console.Write("Id :");
            int id = int.Parse(Console.ReadLine());

            bool b = db.DB_Delete(id);
            try
            {
                if (b == false)
                {
                    Console.WriteLine("삭제실패");
                }
                else
                {
                    Console.WriteLine($"\n{id}번 ID의 정보가 삭제되었습니다");
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
            }
        }
        public void Con_PrintAll()
        {
            List<Book> book_list  = db.DB_PrintAll();
            try
            {
                foreach (Book books in book_list)
                {
                    Console.WriteLine($"{books.id}의 정보 : {books.title}, {books.author},{books.price}");
                }
            }
            catch(Exception ex)
            {
                Console.WriteLine(ex.Message);
            }
          
        }
        #region Connect / Close

        public void Connect()
        {
            db.Connect();
        }

        public void Close()
        {
            db.Close();
        }

        #endregion
    }
}

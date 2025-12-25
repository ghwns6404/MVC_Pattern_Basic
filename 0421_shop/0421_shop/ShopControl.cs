//BookControl.cs
using System;
using System.Collections.Generic;
using System.Xml.Linq;
using WoosongBit41.Data;
using WoosongBit41.Lib;

namespace _0421_shop
{
    internal class ShopControl
    {
        private MyDB db = new MyDB();

        private MyAPI ma = new MyAPI();

        #region 0. 싱글톤 패턴
        public static ShopControl Singleton { get; } = null;
        static ShopControl() { Singleton = new ShopControl(); }
        private ShopControl()
        {

        }
        #endregion

        #region 1. 기능 메서드
        public void Insert()
        {

            string query = WbLib.InputString("검색어 입력");

            List<Shop> shops = MyAPI.Search(query);
            
            foreach (Shop shop in shops)
            {
                db.InsertShop(shop);
            }
        }
        public void PrintAll()
        {
            List<Shop> shops = db.PrintAll();

            foreach (Shop shop in shops)
            {
                if (shop != null)
                    shop.Println();
                else
                    Console.WriteLine("출력할 데이터가 없습니다!!");
            }
        }
        public void Print_Category()
        {
            List<Shop> shops = db.PrintCat();

            foreach (Shop shop in shops)
            {
                shop.Print2();
            }
        }
        public void Select_Category()
        {
            try
            {
                Console.WriteLine("\n[F3키로 카테고리 검색가능!]\n");

                string category1 = WbLib.InputString("원하는 카테고리 검색");

                List<Shop> shops = db.SelectCat(category1);

                foreach (Shop shop in shops)
                {
                    shop.Println();
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
            }
        }
        public void Select_Name()
        {
            try
            {
                Console.WriteLine("\n[F2키로 전체출력 가능!]\n");

                string title = WbLib.InputString("원하는 단어 검색");

                List<Shop> shops = db.SelectTitle(title);

                foreach (Shop shop in shops)
                {
                    shop.Println();
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
            }
        }
        public void Select_Max()
        {
            List<Shop> shops = db.Select_Max_Price();

            foreach (Shop shop in shops)
            {
                shop.Println();
            }
        }
        public void Select_Min()
        {
            List<Shop> shops = db.Select_Min_Price();

            foreach (Shop shop in shops)
            {
                shop.Println();
            }
        }
        public void Delete()
        {
            bool b = db.DeleteAll();
            if (b == true)
                Console.WriteLine("전체삭제 완료");
            else
                Console.WriteLine("삭제 실패");
        }

        #endregion

        #region 2. 시작과 종료 메서드(DB 연결 및 종료)
        public void Init()
        {
            db.Connect();
        }
        public void Exit()
        {
            db.Close();
        }
        #endregion
    }
}
//Account.cs
using System;
using WoosongBit41.Lib;

namespace WoosongBit41.Data
{
    internal class Account
    {
        #region 1. 맴버필드, 프로퍼티(속성)
        public int Number { get; private set; }
        public string Name { get; private set; }
        public int Balance { get; private set; }
        public DateTime Ctime { get; private set; }
        #endregion

        #region 2. 생성자
        public Account(int _number, string _name, int _balance)
        {
            Number = _number;
            Name = _name;
            Balance = _balance;
            Ctime = DateTime.Now;
        }

        public Account(int _number, string _name, int _balance, DateTime _time)
        {
            Number = _number;
            Name = _name;
            Balance = _balance;
            Ctime = _time;
        }
        #endregion

        #region 3. 기능 메서드
        public void Input_Money(int money)
        {
            if (money <= 0)
                throw new Exception("잘못된 금액");

            Balance = Balance + money;
        }
        public void Output_Money(int money)
        {
            if (money <= 0)
                throw new Exception("잘못된 금액");

            if (money > Balance)
                throw new
                    Exception(string.Format("잔액부족-잔액:{0}원,요청금액:{1}원", Balance, money));

            Balance = Balance - money;
        }
        public void Print()
        {
            Console.Write(Number + "\t");
            Console.Write(Name + "\t");
            Console.Write(Balance + "원\t");
            Console.Write(WbLib.Get_Date(Ctime) + "\t");
            Console.Write(WbLib.Get_Time(Ctime) + "\t");
            Console.WriteLine();
        }
        public void Println()
        {
            Console.WriteLine("[번호] " + Number);
            Console.WriteLine("[이름] " + Name);
            Console.WriteLine("[잔액] " + Balance + "원");
            Console.WriteLine("[날짜] " + WbLib.Get_Date(Ctime));
            Console.WriteLine("[시간] " + WbLib.Get_Time(Ctime));
        }
        #endregion 
    }
}

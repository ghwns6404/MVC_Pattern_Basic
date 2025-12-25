//Human.cs
using System;
using System.Threading;


namespace Physical
{
    public class Human
    {
        public int stamina = 0;
        public int Max_stamina = 100;
        public Human(int stamina)
        {
            this.stamina = stamina;
        }

        #region Human의 행동
        public void Walk()
        {
            if (stamina < 10)
            {
                Stamina_Warning();
                return;
            }

            int walk_distance = WbLib.InputInteger("얼마나 걸으시겠습니까? ");

            if (walk_distance <= 0)
            {
                Console.WriteLine("양수만 입력해주세요!");
                return;
            }

            if (stamina < walk_distance * 10)
            {
                Stamina_Warning();
                return;
            }

            Console.WriteLine("뚜벅뚜벅.....");
            Thread.Sleep(1300);
            Console.WriteLine($"{walk_distance}시간을 걸었습니다 !!");
            stamina -= walk_distance * 10;
        }

        public void Run()
        {
            if (stamina < 10)
            {
                Stamina_Warning();
                return;
            }

            int run_distance = WbLib.InputInteger("얼마나 뛰시겠습니까? ");

            if (run_distance <= 0)
            {
                Console.WriteLine("양수만 입력해주세요!");
                return;
            }

            if (stamina < run_distance * 20)
            {
                Stamina_Warning();
                return;
            }

            Console.WriteLine("아이고 힘들어.....");
            Thread.Sleep(1300);
            Console.WriteLine($"{run_distance}시간을 뛰었습니다 !!");
            stamina -= run_distance * 20;
        }

        public void Sleep()
        {
            //현재 나의 체력이 90 >= 이면 잠 잘수 없음
            if (stamina >= 90)
            {
                Sleep_Warning();
                return;
            }
            int sleep_time = WbLib.InputInteger("얼마나 자겠습니까? ");
            if (sleep_time > 10)
            {
                Sleep_Warning();
                return;
            }
            else if (Max_stamina < sleep_time * 10 || Max_stamina < stamina)
            {
                Sleep_Warning();
                return;
            }
            else
            {
                stamina = Math.Min(stamina + sleep_time * 10, Max_stamina);
                Console.WriteLine($"숙면으로 체력이 {sleep_time * 10} 만큼 회복되었습니다!");
            }
        }
        #endregion

        #region 경고문
        public void Stamina_Warning()
        {
            Console.WriteLine("스태미나가 부족합니다!! 수면을 더 취하세요");
        }
        public void Sleep_Warning()
        {
            Console.WriteLine("잘 수 없어!!");
        }
        #endregion
        public int State()
        {
            return stamina;
        }   //app에 현재 체력을 표시하게 해줌

    }
}

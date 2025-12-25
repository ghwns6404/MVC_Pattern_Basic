using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace _0414_람다식
{   

    internal class Program
    {
        delegate int Calculate(int a, int b);
        static int add(int n1, int n2) { return n1 + n2; }

        static void exam1()
        {
            Calculate calc1 = add;
            Console.WriteLine(calc1(10, 20));

            //형식 유추:람다 식 매개 변수를 컴파일러가 유추
            // (매개변수 ) => 문장;
            Calculate calc2 = (int a, int b) => a + b;  
            
            Console.WriteLine(calc2(10, 20));
        }

        #region 1. 기본구조 (매개변수 없고,문장으로 구성된 람다식)
        delegate void DoSomething();
        static void exam2()
        {
            DoSomething DoIt = () => {
                Console.WriteLine("출력1");
                Console.WriteLine("출력2");
            };

            DoIt(); //실행
        }
        #endregion

        #region 2. 기본구조(매개변수사용(타입X), 문장 구성 람다식(return사용))
        delegate int? MyDivide(int a, int b);
        static void exam3()
        {
            MyDivide myFunc = ( a, b) =>
            {
                if (b == 0)
                {
                    return null;
                }
                return a / b;
            };

            Console.WriteLine("10 / 2 == " + myFunc(10, 2)); // 출력 결과: 10 / 2 == 5
            Console.WriteLine("10 / 0 == " + myFunc(10, 0)); // 출력 결과: 10 / 2 ==
        }
        #endregion

        #region 3. 기본구조(매개변수사용(타입X), 문장 구성({ } X, returnX)

        delegate int MyAdd(int a, int b);
        static void exam4()
        {
            MyAdd myFunc = (a, b) => a + b;
            Console.WriteLine("10 + 2 == " + myFunc(10, 2)); // 출력 결과: 10 + 2 == 12
        }
        #endregion

        //---------------------미리 만들어진 delegate사용----------------
        #region 4.Func 사용예
        static void exam5()
        {
            // 입력 매개 변수는 없으며, 반환 타입int  10을 반환
            Func<int> func1 = () => 10;
            Console.WriteLine("func1() : {0}", func1());  //10

            // 입력 매개 변수는 int 형식 하나, 반환 형식도 int
            Func<int, int> func2 = (x) => x * 2;
            Console.WriteLine("func2(4) : {0}", func2(4));     //6

            // 입력 매개 변수는 double, double 형식 두개, 반환 형식도 double
            Func<double, double, double> func3 = (x, y) => x / y;
            Console.WriteLine("func3(22/7) : {0}", func3(22, 7));    //6
        }

        #endregion

        #region 5. Action사용예
        static void exam6()
        {
            Action act1 = () => Console.WriteLine("Action()");
            act1();

            Action<double, double> act3 =  (x, y) =>
                  {
                    double pi = x / y;
                    Console.WriteLine("Action<T1, T2>({0}, {1}) : {2}", x, y, pi);
                  };

            act3(22.0, 7.0);
        }
        #endregion 

        static void Main(string[] args)
        {
            exam6();
        }
    }
}

package org.example;

import java.util.Scanner;

public class App
{
    WbLib lib = new WbLib();
    Scanner sc = new Scanner(System.in);
    Control con = new Control();

    public void Init()
    {
        lib.Logo();
    }

    public void Run()
    {
        lib.Menu();

        while (true)
        {
            System.out.print("Choose Functions >> ");
            int sel = sc.nextInt();
            switch (sel)
            {
                case 1: con.con_make_account();break;
                case 2: con.con_delete_account();break;
                case 3: con.con_select_account();break;
                case 4: con.con_input_money(); break;
                case 5: con.con_output_money(); break;
                case 6: con.con_sendMoney();  break;
                case 7: con.con_printAll(); break;
                case 0: System.out.println("END");return;
                default: System.out.println("Wrong Choice");
            }
        }
    }
    public void Exit()
    {
        lib.Ending();
    }
}





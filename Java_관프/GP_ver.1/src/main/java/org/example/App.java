package org.example;

import java.util.Scanner;

public class App
{
    public WbLib lib = new WbLib();
    public Scanner sc = new Scanner(System.in);
    public Control control = new Control();
    public String userinput;

    public void Init()
    {
        lib.Logo();
    }

    public void Run()
    {
        while(true)
        {
            String menuKey = lib.Menuprint(userinput);  // 메뉴 입력
            //lib.ConsoleClear();

            switch(menuKey)
            {
                case "1" : control.con_Insert(); break;
                case "2" : control.con_Select(); break;
                case "3" : control.con_Update(); break;
                case "4" : control.con_Delete(); break;
                case "5" : control.con_PrintAll(); break;

                case "q" : Exit(); break;
                case "c" : control.con_Cleaner(); break;
            }
        }
    }

    public void Exit()
    {
        lib.Ending();
    }
}

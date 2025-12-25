package org.example;

import java.util.Scanner;

public class WbLib
{
    Scanner sc = new Scanner(System.in);
    public void Pause()
    {

    }


    public void Logo()
    {
        System.out.println("------------");
        System.out.println("GP START");
        System.out.println("------------");
    }

    public String Menuprint(String cs)
    {
        System.out.println("[1] - Insert");
        System.out.println("[2] - Select");
        System.out.println("[3] - Update");
        System.out.println("[4] - Delete");
        System.out.println("[5] - PrintAll");
        System.out.println("[ Q ] - Program Exit");
        System.out.println("[ C ] - Console Clear");

        String userinput =  sc.nextLine();

        return userinput;
    }

    public void Ending()
    {
        System.out.println("------------");
        System.out.println("GP END");
        System.out.println("------------");
    }

}

package org.example;

import java.util.ArrayList;
import java.util.Scanner;

public class Control
{
    ArrayList<Student> stuList = new ArrayList<>();
    Scanner sc = new Scanner(System.in);


    public void con_Insert()
    {
        try
        {
            System.out.println("-----------INSERT-----------");

            System.out.print("insert Number :");
            int number = sc.nextInt();

            System.out.print("insert Name :");
            String name = sc.next();

            System.out.print("insert Address :");
            String addr = sc.next();

            Student stu = new Student(number, name, addr);

            stuList.add(stu);

            System.out.println(" insert OK");
        }
        catch(NumberFormatException  e)
        {
            System.out.println("YOU INSERT WRONG DATA");
        }

    }

    public void con_Select()
    {
        if (stuList == null && stuList.isEmpty())
        {
            System.out.println("Empty List!@!#!");
        }

        System.out.println("-----------SELECT-----------");

        System.out.print("insert Number :");
        int sel_number = sc.nextInt();

        try
        {
            for(Student stu : stuList)
            {
                if(sel_number == stu.Number)
                {
                    System.out.printf("number :%d    name :%s    addr :%s\n", stu.Number, stu.Name, stu.Addr);
                }
                //break;
            }
            System.out.println("select OK");
        }
        catch(Exception ex)
        {
            System.out.println(ex);
        }

    }

    public void con_Update()
    {
        if (stuList == null && stuList.isEmpty())
        {
            System.out.println("Empty List!@!#!");
        }

        System.out.println("--------------UPDATE--------------");

        System.out.print("insert Number :");
        int update_num = sc.nextInt();

        try
        {
            for (Student stu : stuList) {
                if (update_num == stu.Number) {
                    System.out.print("insert NEW Number :");
                    stu.Number = sc.nextInt();

                    System.out.print("insert NEW Name :");
                    stu.Name = sc.next();

                    System.out.print("insert NEW Addr :");
                    stu.Addr = sc.next();

                    System.out.println("UPDATE OK");
                }
            }
        }
        catch(Exception ex)
        {
            System.out.println(ex);
        }
    }

    public void con_Delete()
    {
        if (stuList == null && stuList.isEmpty())
        {
            System.out.println("Empty List!@!#!");
        }

        try
        {
            System.out.println("-----------DELETE-----------");

            System.out.print("insert Number :");
            int del_num = sc.nextInt();
            boolean removed = stuList.removeIf(st -> st.Number == del_num);

            if (removed == true)
            {
                System.out.println("DELETE OK");
            }
            else
            {
                System.out.println("DELETE FAIL");
            }

        }
        catch(NumberFormatException  e)
        {
            System.out.println("YOU DIDN'T DELETE");
        }
    }

    public void con_PrintAll()
    {
        System.out.println("-----------SELECT-----------");
        for (Student st : stuList)
        {
            System.out.printf("number = %d   name = %s   addr = %s\n", st.Number, st.Name, st.Addr);
        }
        System.out.println("-----------------------------");
    }

    public void con_Cleaner()
    {
        for (int i = 0; i < 100; i++) System.out.println();
    }


}

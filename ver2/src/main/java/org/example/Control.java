package org.example;

import java.util.ArrayList;
import java.util.Scanner;

public class Control
{
    ArrayList<Account> accountList = new ArrayList<>();
    Scanner sc = new Scanner(System.in);

    void con_make_account()//계좌 생성
    {
        System.out.print("Create Your Account Number: ");
        int accNumber = sc.nextInt();

        Account check = findAccount(accNumber);
        if (check != null)
        {
            System.out.println("exist Account Number!!");
            return;
        }

        Account acc = new Account(accNumber);
        accountList.add(acc);

        System.out.println(accNumber + " Create OK");
    }
    void con_delete_account()
    {
        System.out.print("Delete Account Number: ");
        int accNumber = sc.nextInt();

        boolean result = accountList.removeIf(acc -> acc.GetNumber() == accNumber);

        if (result == true)
        {
            System.out.println("delete OK");
        }
        else
        {
            System.out.println("delete Fail!!");
        }
    }
    void con_select_account()//계좌 검색
    {
        System.out.print("Select Account Number: ");
        int accNumber = sc.nextInt();

        Account acc = findAccount(accNumber);

        if (acc == null)
        {
            System.out.println("not exist number!!");
            return;
        }
        else
        {
            System.out.printf("Number : %d          Money : %d", acc.GetNumber(), acc.GetBalance());
        }
    }
    void con_input_money()
    {
        System.out.print("insert your account number: ");
        int accNumber = sc.nextInt();

        Account find = findAccount(accNumber);
        if (find == null)
        {
            System.out.println("not exist account!!");
            return;
        }

        System.out.print("How much ? : ");
        int input = sc.nextInt();

        if (find.IncreaseBalance(input))
            System.out.println("Input OK");
        else
            System.out.println("Input Fail!!");
    }
    void con_output_money()//출금
    {
        System.out.print("insert your account number: ");
        int accNumber = sc.nextInt();

        Account find = findAccount(accNumber);
        if (find == null)
        {
            System.out.println("not exist account!!");
            return;
        }

        System.out.print("How much ? : ");
        int output = sc.nextInt();

        if (find.DecreaseBalance(output))
            System.out.println("Output OK");
        else
            System.out.println("Output Fail!!");
    }
    void con_sendMoney()//이체
    {
        System.out.print("who send ? ");
        int send = sc.nextInt();
        Account sender = findAccount(send); //찾은 계좌
        if(sender == null)
        {
            System.out.println("not exist Account!");
            return;
        }


        System.out.print("who recieve ? ");
        int recieve = sc.nextInt();
        Account receiver = findAccount(recieve);
        if (receiver == null)
        {
            System.out.println("not exist Account!");
            return;
        }

        System.out.print("How much do you send ? ");
        int money  = sc.nextInt();
        //---
        if(sender.DecreaseBalance(money) == false)
        {
            System.out.println("not enough money!");
            return;
        }
        if(receiver.IncreaseBalance(money) == false)
        {
            System.out.println("input fail!");
            sender.IncreaseBalance(money);      //위에서 뺀거 다시 돌려줌. 에러났으니까
            return;
        }
        System.out.println("Send OK");
    }
    void con_printAll()//전체계좌 조회
    {
        System.out.println("Account List");
        if (accountList == null)
        {
            System.out.println("list is Empty!!");
        }
        for(Account acc : accountList)
        {
            System.out.printf("Num :%d       Money :%d \n" , acc.GetNumber(), acc.GetBalance());
        }
    }
    Account findAccount(int accNumber)//계좌 찾아내는 함수
    {
        for (Account acc : accountList)
        {
            if (acc.GetNumber() == accNumber)
                return acc;
        }
        return null;
    }


}



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
            System.out.println("exist Account Number!");
            return;
        }

        Account acc = new Account(accNumber);
        accountList.add(acc);

        System.out.println(accNumber + " Create OK");
    }
    void con_delete_account()//계좌 삭제
    {
        System.out.print("Delete Account Number: ");
        int accNumber = sc.nextInt();

        boolean b = accountList.removeIf(acc -> acc.acc_number == accNumber);
        if(b == true)
            System.out.println("delete OK");
        else
            System.out.println("delete Fail");
    }
    void con_select_account()//계좌 검색
    {
        System.out.print("Select Account Number: ");
        int accNumber = sc.nextInt();

        Account acc = findAccount(accNumber);

        if (acc == null)
        {
            System.out.println("not exist account");
        }
        else
        {
            System.out.printf("Number: %d    Money: %d\n", acc.acc_number, acc.balance);
        }
    }
    void con_input_money()//입금
        {
        System.out.print("insert your account number: ");
        int accNumber = sc.nextInt();

        Account find = findAccount(accNumber);
        if (find == null) {
            System.out.println("not exist account");
            return;
        }

        System.out.print("how much money do you input : ");
        int input = sc.nextInt();

        Input_Account ipacc = new Input_Account(accNumber);

        ipacc.balance = find.balance;
        ipacc.Input_Money(input);
        find.balance = ipacc.balance;
    }
    void con_output_money()//출금
    {
        System.out.print("insert your account number: ");
        int accNumber = sc.nextInt();

        Account find = findAccount(accNumber);
        if (find == null) {
            System.out.println("not exist account");
            return;
        }

        System.out.print("how much money do you output : ");
        int output = sc.nextInt();

        if(find.balance < output)
        {
            System.out.println("you don't have enough money");
        }

        Output_Account ipacc = new Output_Account(accNumber);

        ipacc.balance = find.balance;
        ipacc.Output_Money(output);
        find.balance = ipacc.balance;
    }
    void con_sendMoney()//이체
    {
        System.out.print("who send ? ");
        int send = sc.nextInt();
        Account sender = findAccount(send); //찾은 계좌
        Output_Account outacc = new Output_Account(send); //계좌번호 인자로 줘서 생성
        outacc.balance = sender.balance;    //생성된 객체에다가 아까 찾은 잔액 덮어씌움

        System.out.print("who recieve ? ");
        int recieve = sc.nextInt();
        Account receiver = findAccount(recieve);
        Input_Account inacc = new Input_Account(recieve);
        inacc.balance = receiver.balance;

        System.out.print("How much do you send ? ");
        int money  = sc.nextInt();

        if(outacc.Output_Money(money) == false)//돈 넣으면 - 됨
        {
            System.out.println("Wrong");
            return;
        }

        sender.balance = outacc.balance;    //저장

        if(inacc.Input_Money(money) == false)
        {
            System.out.println("Wrong");
            return;
        }
        receiver.balance = inacc.balance;
        System.out.printf("%d won send OK\n", money);
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
            System.out.printf("Num :%d       Money :%d \n" , acc.acc_number, acc.balance);
        }
    }
    Account findAccount(int accNumber)//계좌 찾아내는 함수
    {
        for (Account acc : accountList)
        {
            if (acc.acc_number == accNumber)
                return acc;
        }
        return null;
    }


}



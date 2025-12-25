package org.example;

public class Account
{
    private int acc_number;
    private int balance;

    public Account(int acc_number)
    {
        this.acc_number = acc_number;
        this.balance = 0;
    }

    public int GetNumber()
    {
        return acc_number;
    }
    public int GetBalance()
    {
        return balance;
    }

    protected void SetBalance(int money)
    {
        this.balance = money;
    }

    protected boolean DecreaseBalance(int money)
    {
        if (money > this.balance)
            return false;
        this.balance -= money;
        return true;
    }
    protected  boolean IncreaseBalance(int money)
    {
        if(money <= 0)
        {
            return false;
        }
        this.balance += money;
        return true;
    }









}

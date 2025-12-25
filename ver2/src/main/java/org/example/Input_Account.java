package org.example;

public class Input_Account extends Account
{
    public Input_Account(int acc_number)
    {
        super(acc_number);
    }

    public boolean Input_Money(int money)
    {
        if (money <= 0)
        {
            System.out.println("insert over 0");
            return false;
        }
        IncreaseBalance(money);
        System.out.println("Input OK");
        return true;
    }
}

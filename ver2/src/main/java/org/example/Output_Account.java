package org.example;

public class Output_Account extends Account
{
    public Output_Account(int acc_number)
    {
        super(acc_number);
    }

    public boolean Output_Money(int money)
    {
        if (DecreaseBalance(money) == false)
        {
            System.out.println("not enough money!");
            return false;
        }
        System.out.println("Output OK");
        return true;
    }
}

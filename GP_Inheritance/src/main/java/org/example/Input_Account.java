package org.example;

public class Input_Account extends Account
{

    public Input_Account(int acc_number)
    {
        super(acc_number);
    }

    public boolean Input_Money(int amount)
    {
        if (amount <= 0)
        {
            System.out.println("insert over 0");
            return false;
        }
        this.balance += amount;
        System.out.println(amount + "won insert OK, Now Balace : " + balance);
        return true;
    }
}

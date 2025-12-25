package org.example;

public class Output_Account extends Account
{
    public Output_Account(int acc_number)
    {
        super(acc_number);
    }

    public boolean Output_Money(int amount)
    {
        if (amount > this.balance )
        {
            System.out.println("you cannot output");
            return false;
        }
        this.balance -= amount;
        System.out.println(amount + "won out OK, Now Balace : " + balance);
        return true;
    }
}

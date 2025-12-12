#include <iostream>
class BankAccount
{
public:
    BankAccount(std::string account_number, std::string owner_name, float balance = 0)
        : account_number(account_number), owner_name(owner_name), balance(balance) {}
    void deposit(float amount)
    {
        if (amount <= 0)
            std::cout << "error: amount must be >0;" << std::endl;
        else
        {
            balance += amount;
            std::cout << "Notification(account "<<account_number<<"): successful deposit; Current balance:" << balance << std::endl;
        }
    }
    void withdraw(float amount)
    {
        if (amount <= 0)
            std::cout << "error: amount must be >0;" << std::endl;
        else if (amount > balance)
            std::cout << "error: amount must be <= balance; balance: " << balance << std::endl;
        else
        {
            balance -= amount;
            std::cout << "Notification(account "<<account_number<<"): successful withdraw; Current balance:" << balance << std::endl;
        }
    }

protected:
    std::string account_number, owner_name;
    float balance;
};
class SavingsAccount : public BankAccount
{
public:
    SavingsAccount(float interest_rate, std::string account_number, std::string owner_name, float balance = 0)
        : interest_rate(interest_rate), BankAccount(account_number, owner_name, balance) {}

    void charge_percentage()
    {
        balance += balance * interest_rate;
        std::cout << "Notification(account "<<account_number<<"): successful interest accrual; Current balance:" << balance << std::endl;
    }

private:
    float interest_rate;
};
int main()
{
    BankAccount banc_acc("00000001", "owner1", 100);
    SavingsAccount save_acc(0.01, "00000002", "owner2");
    banc_acc.withdraw(10);
    save_acc.deposit(10);
    save_acc.charge_percentage();

    return 0;
}
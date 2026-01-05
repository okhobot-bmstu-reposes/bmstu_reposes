#include <payment.hpp>

bool CreditPayment::pay()
{
    float isum;
    std::cout << "The payment method is selected: CreditPayment" << std::endl;
    std::cout << "Confirm the cost of the payment: " << sum << std::endl;
    std::cin >> isum;
    if (isum != sum)
    {
        std::cerr << "the entered amount does not match the expected amount" << std::endl;
        return false;
    }
    return true;
}
bool CashPayment::pay()
{
    float isum;
    std::cout << "The payment method is selected: CashPayment" << std::endl;
    std::cout << "Confirm the cost of the payment: " << sum << std::endl;
    std::cin >> isum;
    if (isum != sum)
    {
        std::cerr << "the entered amount does not match the expected amount" << std::endl;
        return false;
    }
    return true;
}
bool FPS::pay()
{
    float isum;
    std::cout << "The payment method is selected: FPS" << std::endl;
    std::cout << "Confirm the cost of the payment: " << sum << std::endl;
    std::cin >> isum;
    if (isum != sum)
    {
        std::cerr << "the entered amount does not match the expected amount" << std::endl;
        return false;
    }
    return true;
}

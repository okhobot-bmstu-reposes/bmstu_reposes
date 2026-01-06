#include <users/customer.hpp>
#include <payment.hpp>
void Customer::makePayment(unsigned int order_index)
{
    int ps_index;
    std::shared_ptr<PaymentStrategy> payment;

    std::cout << "Доступны способы оплаты:"
              << "\n1. CreditPayment"
              << "\n2. CashPayment"
              << "\n3. FPS" << std::endl;

    std::cout << "Enter the selected payment strategy:" << std::endl;
    std::cin >> ps_index;
    switch (ps_index)
    {
    case 1:
        payment = std::make_shared<CreditPayment>(orders[order_index]->getTotalPrice());
        break;
    case 2:
        payment = std::make_shared<CashPayment>(orders[order_index]->getTotalPrice());
        break;
    case 3:
        payment = std::make_shared<FPS>(orders[order_index]->getTotalPrice());
        break;

    default:
        std::cerr << "There is no payment strategy with an index " << ps_index << std::endl;
        break;
    }
    if (payment->pay())
    {
        dbConn->executeNonQuery("CALL createOrder(3,\'" + orders[order_index]->makeJSON() + "\'::JSONB);");
        orders.erase(orders.begin() + order_index);
    }
}

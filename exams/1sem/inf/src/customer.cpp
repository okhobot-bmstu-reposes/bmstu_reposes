#include <customer.hpp>

void Customer::makePayment(unsigned int order_index)
{
    std::shared_ptr<PaymentStrategy> payment;
    ///
    dbConn->executeNonQuery("CALL createOrder(3,\'"+orders[order_index].makeJSON()+"\'::JSONB);");
    orders.erase(orders.begin()+order_index);
}

#pragma once
#include <iostream>
#include <memory>
#include <pqxx/pqxx>
#include <order.hpp>
#include <payment.hpp>
#include <users/user.hpp>
class Customer:public User
{
public:
    void addToOrder(unsigned int order_index, int product_id, unsigned int quantity)
    {
        orders[order_index]->addOrderItem(
            std::make_shared<OrderItem>(product_id, quantity,
                dbConn->executeQuery("SELECT price FROM products WHERE product_id="+std::to_string(product_id))[0]["price"].as<float>()
            )
        );
    }
    void removeFromOrder(unsigned int order_index, int product_id)
    {
        orders[order_index]->removeOrderItem(product_id);
    }
    void makePayment(unsigned int order_index);
};

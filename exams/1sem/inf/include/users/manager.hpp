#pragma once

#include <iostream>
#include <memory>
#include <pqxx/pqxx>
#include <order.hpp>
#include <payment.hpp>
#include <users/user.hpp>
class Manager : public User
{
public:
    void approveOrder(int id)
    {
        dbConn->executeNonQuery("CALL updateOrderStatus(" + std::to_string(id) + ", \'completed\', 1);");
    }
    void updateStock(int id, unsigned int new_stock_quantity)
    {
        dbConn->executeNonQuery("UPDATE products SET stock_quantity=" + new_stock_quantity + " WHERE product_id=" + id + ";");
    }
};

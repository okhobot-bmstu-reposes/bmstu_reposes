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
    Manager(std::shared_ptr<DatabaseConnection> a_dbConn) : User(a_dbConn) {}
    void approveOrder(int id)
    {
        dbConn->executeNonQuery("CALL updateOrderStatus(" + std::to_string(id) + ", \'completed\', 1);");
    }
    void updateStock(int id, unsigned int new_stock_quantity)
    {
        dbConn->executeNonQuery("UPDATE products SET stock_quantity=" + std::to_string(new_stock_quantity) + " WHERE product_id=" + std::to_string(id) + ";");
    }
    void updateOrderStatus(int id, std::string new_status)
    {
        std::string old_status = getOrderStatus(id);
        if ((new_status == "pending" || new_status == "complited") && (old_status == "pending" || old_status == "complited"))
            dbConn->executeNonQuery("CALL updateOrderStatus(" + std::to_string(id) + ", \'" + new_status + "\', 2);");
        else
            std::cerr << "Вы можете установить статус \"pending\" или \"complited\", если заказ имеет один из этих статусов";
    }
};

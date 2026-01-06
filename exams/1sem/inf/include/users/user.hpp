#pragma once

#include <iostream>
#include <memory>
#include <pqxx/pqxx>
#include <order.hpp>
#include <payment.hpp>
#include <order.hpp>
#include <databaseConnection.hpp>
class User
{
public:
    void createOrder() { orders.push_back(std::make_shared<Order>()); }
    void viewOrderStatus(int orderId)
    {
        std::cout<<dbConn->executeQuery("SELECT getOrderStatus(" + std::to_string(orderId) + ")")[0]["getOrderStatus"].as<std::string>()<<std::endl;
    }
    void cancelOrder(int orderId)
    {
        dbConn->executeNonQuery("UPDATE orders SET status = 'canceled' WHERE order_id=" + std::to_string(orderId) + ";");
    }
    std::string getOrderStatus(int orderId)
    {
        return dbConn->executeQuery("SELECT getOrderStatus(" + std::to_string(orderId) + ")")[0]["getOrderStatus"].as<std::string>();
    }
    void viewOrders()
    {
        for(int i=0;i<orders.size();i++)
        {
            std::cout<<"Индекс заказа: "<<i<<std::endl;
            orders[i]->printOrder();
            std::cout<<"---------------"<<i<<std::endl;
        }

    }

    User(std::shared_ptr<DatabaseConnection> a_dbConn) { dbConn = a_dbConn;}

protected:
    std::string name, email, role, password_hash;
    int id, loyalty_level;
    std::vector<std::shared_ptr<Order>> orders;
    std::shared_ptr<DatabaseConnection> dbConn;
};

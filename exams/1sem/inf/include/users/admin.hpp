#pragma once
#include <iostream>
#include <memory>
#include <pqxx/pqxx>
#include <order.hpp>
#include <product.hpp>
#include <users/user.hpp>
class Admin : public User
{
public:
    Admin(std::shared_ptr<DatabaseConnection> a_dbConn) : User(a_dbConn) {}
    void addProduct(Product p) { dbConn->executeNonQuery("INSERT INTO products (name, price, stock_quantity) VALUES " + p.getInsertString() + ";"); }
    void deleteProduct(int product_id) { dbConn->executeNonQuery("DELETE FROM products WHERE product_id=" + std::to_string(product_id) + ";"); }
    void updateOrderStatus(int id, std::string new_status) { dbConn->executeNonQuery("CALL updateOrderStatus(" + std::to_string(id) + ", \'" + new_status + "\', 1);"); }
    void updateProduct(int id, Product new_p)
    {
        dbConn->executeNonQuery(
            "UPDATE products SET stock_quantity=" + std::to_string(new_p.getStockQuantity()) +
            ", price=" + std::to_string(new_p.getPrice()) +
            ", name= \'" + new_p.getName() +
            "\' WHERE product_id=" + std::to_string(id) + ";");
    }

    void viewAllOrders()
    {
        auto orders = dbConn->executeQuery("SELECT * FROM orders;");
        for (int i = 0; i < orders.columns(); ++i)
            std::cout << std::left << std::setw(16) << orders.column_name(i) << "|";
        std::cout << std::endl;
        for (const auto &order : orders)
        {
            for (auto it = order.begin(); it != order.end(); it++)
                std::cout << std::left << std::setw(16) << it->as<std::string>() << "|";
            std::cout << std::endl;
        }
    }
};

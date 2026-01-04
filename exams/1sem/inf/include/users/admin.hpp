#include <iostream>
#include <memory>
#include <pqxx/pqxx>
#include <order.hpp>
#include <product.hpp>
#include <users/user.hpp>
class Admin : public User
{
public:
    void addProduct(Product p) { dbConn->executeNonQuery("INSERT INTO products (name, price, stock_quantity) VALUES " + p.getInsertString() + ";"); }
    void deleteProduct(int product_id) { dbConn->executeNonQuery("DELETE FROM products WHERE product_id=" + std::to_string(product_id) + ";"); }
    void updateOrderStatus(int id, std::string new_status) { dbConn->executeNonQuery("UPDATE orders SET status=new_status WHERE irder_id=" + std::to_string(id) + ";"); }
    void updateProduct(int id, unsigned int new_stock_quantity) { dbConn->executeNonQuery("UPDATE products SET stock_quantity=" + new_stock_quantity + " WHERE product_id="+id+";"); }

    void viewAllOrders()
    {
        auto orders = dbConn->executeQuery("SELECT * FROM orders;");
        for (const auto &order : orders)
        {
            for (auto it = order.begin(); it != order.end(); i++)
                std::cout << it->first << ": " << it->second << "; ";
            std::cout << std::endl;
        }
    }
};

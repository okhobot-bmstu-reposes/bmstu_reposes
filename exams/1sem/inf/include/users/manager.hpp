#include <iostream>
#include <memory>
#include <pqxx/pqxx>
#include <order.hpp>
#include <payment.hpp>
#include <users/user.hpp>
class Manager:public User
{
public:
    void approveOrder(int id) { dbConn->executeNonQuery("UPDATE orders SET status=\'completed\' WHERE irder_id=" + std::to_string(id) + ";"); }
    void updateStock(int id, unsigned int new_stock_quantity) { dbConn->executeNonQuery("UPDATE products SET stock_quantity=" + new_stock_quantity + " WHERE product_id="+id+";"); }

};

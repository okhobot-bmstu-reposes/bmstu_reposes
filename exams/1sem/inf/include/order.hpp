#include <iostream>
#include <memory>
#include <pqxx/pqxx>

class OrderItem
{
    public:
    private:
    int order_item_id, order_id, product_id;
    unsigned int quantity;
    float price;
};

class Order
{
    std::vector<std::shared_ptr<OrderItem>> orderItems;
    public:
    void addOrderItem(std::shared_ptr<OrderItem> item);
    void removeOrderItem(unsigned int index);
};

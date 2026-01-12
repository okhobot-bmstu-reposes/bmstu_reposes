#pragma once
#include <iostream>
#include <memory>
#include <pqxx/pqxx>
#include <algorithm>

class OrderItem
{
public:
    OrderItem(int product_id, unsigned int quantity, float price) : product_id(product_id), quantity(quantity), price(price) {}
    std::string getJSON()
    {
        return "{\"product_id\": " + std::to_string(product_id) + ", \"quantity\": " + std::to_string(quantity) + "}";
    }
    std::string getString()
    {
        return "product_id: " + std::to_string(product_id) + "; quantity: " + std::to_string(quantity) + "; price: " + std::to_string(price) + ";";
    }
    float getPrice()
    {
        return price;
    }
    unsigned int getQuantity()
    {
        return quantity;
    }

private:
    int order_item_id, order_id, product_id;
    unsigned int quantity;
    float price;
};

class Order
{
    std::vector<std::shared_ptr<OrderItem>> orderItems;

public:
    void addOrderItem(std::shared_ptr<OrderItem> item) { orderItems.push_back(item); }
    void removeOrderItem(unsigned int index) { orderItems.erase(orderItems.begin() + index); }
    void printOrder()
    {
        for (int i = 0; i < orderItems.size(); i++)
            std::cout << orderItems[i]->getString() << std::endl;
    }
    int getTotalPrice()
    {
        return std::accumulate(orderItems.begin(), orderItems.end(), 0,
                               [](int total, const std::shared_ptr<OrderItem> &item)
                               {
                                   return total + item->getPrice() * item->getQuantity();
                               });
    }
    std::string makeJSON()
    {
        std::string json = "[\n";
        for (int i = 0; i < orderItems.size(); i++)
            json += (i > 0 ? ",\n" : "\n") + orderItems[i]->getJSON();
        json += "]";
        return json;
    }
};

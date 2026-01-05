#pragma once

#include <iostream>
#include <memory>
#include <pqxx/pqxx>

class Product
{
    public:
    Product(unsigned int stock_quantity, float price, std::string name):stock_quantity(stock_quantity),price(price),name(name){}
    std::string getInsertString()
    {
        return "(\""+name+"\", "+std::to_string(price)+", "+std::to_string(stock_quantity)+")";
    }

    private:
    int product_id;
    unsigned int stock_quantity;
    float price;
    std::string name;
};

#pragma once

#include <iostream>
#include <memory>
#include <pqxx/pqxx>

class Product
{
public:
    Product(unsigned int stock_quantity, float price, std::string name) : stock_quantity(stock_quantity), price(price), name(name) {}
    std::string getInsertString()
    {
        return "(\'" + name + "\', " + std::to_string(price) + ", " + std::to_string(stock_quantity) + ")";
    }
    Product()
    {
        std::cout << "Введите количество на складе, цену и имя(без пробелов) продукта"<<std::endl;
        std::cin>>stock_quantity>>price>>name;
    }
    int getId() { return product_id; }
    unsigned int getStockQuantity() { return stock_quantity; }
    float getPrice() { return price; }
    std::string getName() { return name; }

    void setId(int id) { product_id = id; }

private:
    int product_id;
    unsigned int stock_quantity;
    float price;
    std::string name;
};

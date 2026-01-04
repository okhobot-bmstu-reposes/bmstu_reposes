#include <iostream>
#include <memory>
#include <pqxx/pqxx>

class Product
{
    public:
    std::string getInsertString()
    {
        return "(\""+name+"\", "+price+", "+stock_quantity+")";
    }
    private:
    int product_id;
    unsigned int stock_quantity;
    float price;
    std::string name;
};

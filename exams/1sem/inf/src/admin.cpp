#include <users/admin.hpp>
void Admin::viewOrders()
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

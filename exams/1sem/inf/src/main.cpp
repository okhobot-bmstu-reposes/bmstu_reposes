#include <iostream>
#include <pqxx/pqxx> // Подключение библиотеки
#include <sstream>
#include <fstream>
#include <windows.h>
#include <locale.h>

#include <users/admin.hpp>
#include <users/customer.hpp>
#include <users/manager.hpp>

#include <databaseConnection.hpp>

static const std::string conn_info = "host=localhost port=5432 dbname=o_store user=postgres password=301";

void admin_com()
{
    std::shared_ptr<DatabaseConnection> dbConn = std::make_shared<DatabaseConnection>(conn_info);
    Admin user(dbConn);
    bool run = true;

    while (run)
    {
        std::cout << "Меню администратора:"
                  << "\n1. Добавить новый продукт"
                  << "\n2. Обновить информацию о продукте"
                  << "\n3. Удалить продукт"
                  << "\n4. Просмотр всех заказов"
                  << "\n5. Просмотр деталей заказа"
                  << "\n6. Изменить статус заказа"
                  << "\n7. Просмотр истории статусов заказа"
                  << "\n8. Просмотр журнала аудита"
                  << "\n9. Сформировать отчёт (CSV)"
                  << "\n10. Выход" << std::endl;
        int com;
        std::cin >> com;
        switch (com)
        {
        case 1:
        {
            Product p;
            user.addProduct(p);
            break;
        }
        case 2:
        {
            int id;
            std::cout << "Введите id продукта" << std::endl;
            std::cin >> id;
            Product p;
            user.updateProduct(id, p);
            break;
        }
        case 3:
        {
            int id;
            std::cout << "Введите id продукта" << std::endl;
            std::cin >> id;
            user.deleteProduct(id);
            break;
        }
        case 4:
        {
            user.viewAllOrders();
            break;
        }
        case 5:
        {
            int id;
            std::cout << "Введите id заказа" << std::endl;
            std::cin >> id;
            user.viewOrderStatus(id);
            break;
        }
        case 6:
        {
            int id;
            std::string new_status;
            std::cout << "Введите id и новый статус заказа" << std::endl;
            std::cin >> id >> new_status;
            user.updateOrderStatus(id, new_status);
            break;
        }
        case 7:
        {
            int id;
            std::string new_status;
            std::cout << "Введите id заказа" << std::endl;
            std::cin >> id;
            auto history = dbConn->executeQuery("SELECT getOrderStatusHistory(" + std::to_string(id) + ");");
            std::cout << "(old_status, new_status, changed_at(date time), changed_by)" << std::endl;
            for (auto &line : history)
                std::cout << line["getOrderStatusHistory"].as<std::string>() << std::endl;
            break;
        }

        default:
            run = false;
            break;
        }
        std::cout << "успешно" << std::endl;
    }
}
void manager_com() {};
void customer_com() {};
int main()
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    std::setlocale(LC_ALL, "ru_RU.UTF-8");
    std::locale::global(std::locale("C"));

    std::cout << "Пожалуйста, выберите свою роль:\n1. Войти как Администратор\n2. Войти как Менеджер\n3. Войти как Покупатель\n4. Выход " << std::endl;

    int com;
    std::cin >> com;
    switch (com)
    {
    case 1:
        admin_com();
        break;
    case 2:
        manager_com();
        break;
    case 3:
        customer_com();
        break;

    default:
        break;
    }

    return 0;
}

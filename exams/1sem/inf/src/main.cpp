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

void printTable(pqxx::result data)
{
    for (int i = 0; i < data.columns(); ++i)
        std::cout << std::left << std::setw(16) << data.column_name(i) << "|";
    std::cout << std::endl;
    for (const auto &el : data)
    {
        for (auto it = el.begin(); it != el.end(); it++)
            std::cout << std::left << std::setw(16) << it->as<std::string>() << "|";
        std::cout << std::endl;
    }
}

void admin_com()
{
    std::shared_ptr<DatabaseConnection> dbConn = std::make_shared<DatabaseConnection>(conn_info);
    Admin user(dbConn);
    bool run = true;

    while (run)
        try
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
                std::cout << "*(old_status, new_status, changed_at(date time), changed_by)*" << std::endl;
                for (auto &line : history)
                    std::cout << line["getOrderStatusHistory"].as<std::string>() << std::endl;
                break;
            }
            case 8:
            {
                int id;
                std::string new_status;
                std::cout << "Введите id пользователя" << std::endl;
                std::cin >> id;
                auto history = dbConn->executeQuery("SELECT getAuditLogByUser(" + std::to_string(id) + ");");
                std::cout << "*(entity_type, entity_id, operation, changed_at(date time))*" << std::endl;
                for (auto &line : history)
                    std::cout << line["getAuditLogByUser"].as<std::string>() << std::endl;
                break;
            }
            case 9:
            {
                auto history = dbConn->executeQuery("SELECT generateCSVReport(1);");
                std::ofstream fout("history.csv");
                fout << history[0]["generateCSVReport"].as<std::string>();
                fout.close();
                break;
            }

            default:
                run = false;
                break;
            }
            std::cout << "done" << std::endl;
        }
        catch (const std::exception &e)
        {
            std::cerr << (std::string("Ошибка: ") + e.what()) << std::endl;
        }
}
void manager_com()
{
    std::shared_ptr<DatabaseConnection> dbConn = std::make_shared<DatabaseConnection>(conn_info);
    Manager user(dbConn);
    bool run = true;

    while (run)
        try
        {
            std::cout << "Меню менеджера:"
                      << "\n1. Просмотр заказов в ожидании утверждения"
                      << "\n2. Утвердить заказ"
                      << "\n3. Обновить количество товара на складе"
                      << "\n4. Просмотр статуса заказа"
                      << "\n5. Изменить статус заказа (в рамках полномочий)"
                      << "\n6. Просмотр истории аудита менеджера"
                      << "\n7. Просмотр истории статусов заказов"
                      << "\n8. Выход" << std::endl;
            int com;
            std::cin >> com;
            switch (com)
            {
            case 1:
            {
                auto orders = dbConn->executeQuery("SELECT order_id, user_id, total_price, order_date FROM orders WHERE status=\'pending\'");
                printTable(orders);
                break;
            }
            case 2:
            {
                int id;
                std::cout << "Введите id заказа" << std::endl;
                std::cin >> id;
                user.approveOrder(id);
                break;
            }
            case 3:
            {
                int id, new_stock_quantity;
                std::cout << "Введите id товара и новое значение stock_quantity" << std::endl;
                std::cin >> id >> new_stock_quantity;
                user.updateStock(id, new_stock_quantity);
                break;
            }
            case 4:
            {
                int id;
                std::cout << "Введите id заказа" << std::endl;
                std::cin >> id;
                user.viewOrderStatus(id);
                break;
            }
            case 5:
            {
                int id;
                std::string new_status;
                std::cout << "Введите id и новый статус заказа" << std::endl;
                std::cin >> id >> new_status;
                user.updateOrderStatus(id, new_status);
                break;
            }
            case 6:
            {
                printTable(dbConn->executeQuery("SELECT * FROM audit_log WHERE performed_by = 2"));
                break;
            }
            case 7:
            {
                printTable(dbConn->executeQuery("SELECT * FROM order_status_history WHERE changed_by = 2"));
                break;
            }

            default:
                run = false;
                break;
            }
            std::cout << "done" << std::endl;
        }
        catch (const std::exception &e)
        {
            std::cerr << (std::string("Ошибка: ") + e.what()) << std::endl;
        }
}
void customer_com()
{
    std::shared_ptr<DatabaseConnection> dbConn = std::make_shared<DatabaseConnection>(conn_info);
    Customer user(dbConn);
    bool run = true;

    while (run)

        {
            std::cout << "Меню покупателя:"
                      << "\n1. Создать новый заказ"
                      << "\n2. Добавить товар в заказ"
                      << "\n3. Удалить товар из заказа"
                      << "\n4. Просмотр моих заказов"
                      << "\n5. Просмотр статуса заказа"
                      << "\n6. Оплатить заказ"
                      << "\n7. Оформить возврат заказа"
                      << "\n8. Просмотр истории статусов заказа"
                      << "\n9. Выход" << std::endl;
            int com;
            std::cin >> com;
            switch (com)
            {
            case 1:
            {
                user.createOrder();
                break;
            }
            case 2:
            {
                unsigned int index, quantity;
                int p_id;
                std::cout << "Введите индекс заказа, id товара и его количество" << std::endl;
                std::cin >> index >> p_id >> quantity;
                user.addToOrder(index, p_id, quantity);
                break;
            }
            case 3:
            {
                unsigned int index;
                int p_id;
                std::cout << "Введите индекс заказа и id товара" << std::endl;
                std::cin >> index >> p_id;
                user.removeFromOrder(index, p_id);
                break;
            }
            case 4:
            {
                std::cout << "Неоплаченные заказы:" << std::endl;
                user.viewOrders();
                std::cout << std::endl
                          << "Оплаченные заказы:" << std::endl;
                printTable(dbConn->executeQuery("SELECT * FROM orders WHERE user_id = 3"));
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
                int index;
                std::cout << "Введите индекс заказа" << std::endl;
                std::cin >> index;
                user.makePayment(index);
                break;
            }
            case 7:
            {
                int id;
                std::cout << "Введите id заказа" << std::endl;
                std::cin >> id;
                if (dbConn->executeQuery("SELECT canReturnOrder(" + std::to_string(id) + ",3);")[0]["canReturnOrder"].as<bool>())
                    dbConn->executeNonQuery("CALL updateOrderStatus(" + std::to_string(id) + ", \'returned\', 1);");
                else
                    std::cerr << "Возврат этого заказа недоступен" << std::endl;
                break;
            }
            case 8:
            {
                printTable(dbConn->executeQuery(
                    "SELECT history_id, order_status_history.order_id, old_status, new_status,changed_by,changed_at "
                    "FROM order_status_history INNER JOIN orders ON order_status_history.order_id = orders.order_id "
                    "WHERE orders.user_id=3;"));
                break;
            }

            default:
                run = false;
                break;
            }
            std::cout << "done" << std::endl;
        }

}
int main()
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    std::setlocale(LC_ALL, "ru_RU.UTF-8");
    std::locale::global(std::locale("C"));


    int com;
    bool run = true;
    while (run)
    {
        std::cout << "Пожалуйста, выберите свою роль:\n1. Войти как Администратор\n2. Войти как Менеджер\n3. Войти как Покупатель\n4. Выход " << std::endl;
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
            run = false;
            break;
        }
    }

    return 0;
}

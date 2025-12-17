#include <user.hpp>
void User::save(pqxx::connection &conn, std::ostream &outp)
{
    try
    {
        // Начинаем транзакцию
        pqxx::work txn(conn);

        // SQL запрос с параметрами для предотвращения SQL-инъекций
        std::string sql =
            "INSERT INTO users (id, name, registration_date) "
            "VALUES (" +
            std::to_string(id) + ", " +
            txn.quote(name) + ", " +
            txn.quote(registration_date) +
            ")";

        //std::cout << "SQL запрос:" << std::endl;
        //std::cout << sql << std::endl;

        // Выполняем запрос
        txn.exec(sql);

        // Подтверждаем транзакцию
        txn.commit();

        outp << "Пользователь "<<name<<"("<<id<<") успешно сохранен" << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        throw; // Пробрасываем исключение дальше
    }
}
User::User(const pqxx::result &result, int index)
{
    try
    {
        id = result[index]["id"].as<int>();
        name = result[index]["name"].as<std::string>();
        registration_date = result[index]["registration_date"].as<std::string>();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        throw;
    }
}

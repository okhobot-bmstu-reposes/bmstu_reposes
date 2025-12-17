#include <author.hpp>
void Author::save(pqxx::connection &conn, std::ostream &outp)
{
    try
    {
        // Начинаем транзакцию
        pqxx::work txn(conn);

        // SQL запрос с параметрами для предотвращения SQL-инъекций
        std::string sql =
            "INSERT INTO authors (id, name, birth_year) "
            "VALUES (" +
            std::to_string(id) + ", " +
            txn.quote(name) + ", " +
            std::to_string(birth_year) +
            ")";

        //std::cout << "SQL запрос:" << std::endl;
        //std::cout << sql << std::endl;

        // Выполняем запрос
        txn.exec(sql);

        // Подтверждаем транзакцию
        txn.commit();

        outp << "Автор "<<name<<"("<<id<<") успешно сохранен" << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        throw; // Пробрасываем исключение дальше
    }
}
Author::Author(const pqxx::result &result, int index)
{
    try
    {
        id = result[index]["id"].as<int>();
        name = result[index]["name"].as<std::string>();
        birth_year = result[index]["birth_year"].as<int>();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        throw;
    }
}

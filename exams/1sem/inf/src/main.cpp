#include <iostream>
#include <pqxx/pqxx> // Подключение библиотеки
#include <sstream>
#include <fstream>

pqxx::result request(std::string sql, pqxx::connection &conn)
{
    // Начинаем транзакцию
    pqxx::work txn(conn);

    // Выполняем запрос
    pqxx::result result = txn.exec(sql);

    // Подтверждаем транзакцию
    txn.commit();

    return result;
}
int get_id(std::string tname, pqxx::connection &conn)
{
    int id = request("SELECT MAX(id) AS max_id FROM " + tname, conn)[0]["max_id"].as<int>();
    if (id <= 0)
        id = 1;
    return ++id;
}
int main()
{
    try
    {
        // Строка подключения к базе данных
        std::string conninfo = "host=localhost port=5432 dbname=library user=postgres password=301";

        // Устанавливаем соединение с базой данных
        pqxx::connection conn(conninfo);
        std::ofstream outp;

        if (conn.is_open())
        {
            std::cout << "Connected to database: " << conn.dbname() << std::endl;
            // Book b(1, "b1", 1, 1, "g1");
            // Author a(1, "name", 0);
            // a.save(conn);
            // b.save(conn);
            //  Book b(conn, 1);
            //  Author a(conn,1);
            // User u(1, "u1", "2025-12-10");
            // u.save(conn);
            //  User u(conn,1);
            // Borrowed_book bb(1, 1, "2025-12-10", "NULL");
            // bb.save(conn);
        }
        else
        {
            std::cerr << "Failed to connect to the database." << std::endl;
            return 1;
        }

        // Закрываем соединение
        conn.close();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

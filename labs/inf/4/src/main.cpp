#include <iostream>
#include <book.hpp>
#include <author.hpp>
#include <user.hpp>
#include <borrowed_book.hpp>
#include <pair.hpp>
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

            std::string com;
            std::stringstream logs;
            do
            {
                logs.str("");
                std::cin >> com;
                if (com == "help")
                {
                    logs << "call: " << com << "\nresult:\n";
                    logs << "Доступные команды:\n"
                         << "  help                         - показать это сообщение\n"
                         << "  add_author <имя> <год_рождения>      - добавить нового автора\n"
                         << "  add_book <название> <id_автора> <год_издания> <жанр> - добавить книгу\n"
                         << "  add_user <имя> <дата_регистрации(YYYY-MM-DD)> - добавить пользователя\n"
                         << "  add_borrowed <id_пользователя> <id_книги> <дата_взятия> <дата_возврата/NULL> - записать выдачу книги\n\n"
                         << "  get_author_books <имя_автора>        - получить книги автора\n"
                         << "  new_users                            - показать новых пользователей (за последний год)\n"
                         << "  last_borrowed                        - показать последние выдачи (за 30 дней)\n"
                         << "  most_popular                         - показать 3 самые популярные книги\n"
                         << "  exit                                 - выход из программы"
                         << std::endl;
                }
                else if (com == "add_author")
                {
                    Author a(get_id("authors",conn));
                    a.save(conn, std::cout);
                }
                else if (com == "add_book")
                {
                    Book b(get_id("books",conn));
                    b.save(conn, std::cout);
                }
                else if (com == "add_user")
                {
                    User u(get_id("users",conn));
                    u.save(conn, std::cout);
                }
                else if (com == "add_borrowed")
                {
                    Borrowed_book bb;
                    bb.save(conn, std::cout);
                }
                else if (com == "get_author_books")
                {
                    logs << "call: " << com << "\nresult:\n";
                    std::cin >> com;
                    pqxx::result res = request(
                        "SELECT b.id, b.title FROM books b "
                        "JOIN authors a ON b.author_id = a.id "
                        "WHERE a.name = \'" +
                            com + "\';",
                        conn);

                    for (int i = 0; i < res.size(); i++)
                        logs << res[i]["title"].as<std::string>() << "(" << res[i]["id"].as<std::string>() << ")\n";
                }
                else if (com == "new_users")
                {
                    pqxx::result res = request(
                        "SELECT COUNT(*) as user_count "
                        "FROM users WHERE registration_date >= CURRENT_DATE - INTERVAL \'1 year\';",
                        conn);
                    logs << "call: " << com << "\nresult:\n";
                    if (!res.empty())
                        logs << res[0]["user_count"].as<std::string>() << "\n";
                }
                else if (com == "last_borrowed")
                {
                    pqxx::result res = request(
                        "SELECT b.id, b.title "
                        "FROM borrowed_books bb "
                        "JOIN books b ON bb.book_id = b.id "
                        "WHERE bb.borrow_date >= CURRENT_DATE - INTERVAL \'30 days\';",
                        conn);
                    logs << "call: " << com << "\nresult:\n";
                    for (int i = 0; i < res.size(); i++)
                        logs << res[i]["title"].as<std::string>() << "(" << res[i]["id"].as<std::string>() << ")\n";
                }
                else if (com == "most_popular")
                {
                    pqxx::result res = request("SELECT b.id, b.title, "
                                               "COUNT(bb.book_id) as borrow_count "
                                               "FROM books b "
                                               "JOIN borrowed_books bb ON b.id = bb.book_id "
                                               "GROUP BY b.id, b.title "
                                               "ORDER BY borrow_count DESC "
                                               "LIMIT 3;",
                                               conn);
                    logs << "call: " << com << "\nresult:\n";
                    for (int i = 0; i < res.size(); i++)
                        logs << res[i]["title"].as<std::string>() << "(" << res[i]["id"].as<std::string>() << ")\n";
                }
                else
                    logs << "call: " << com << ";";
                std::cout << logs.str() << std::endl;

                outp.open("logs.txt", std::ios::app);
                outp << logs.str() << std::endl;
                outp.close();

            } while (com != "exit");
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

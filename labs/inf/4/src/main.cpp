#include <iostream>
#include <book.hpp>
#include <author.hpp>
#include <user.hpp>
#include <borrowed_book.hpp>
#include <pqxx/pqxx> // Подключение библиотеки

int main() {
    try {
        // Строка подключения к базе данных
        std::string conninfo = "host=localhost port=5432 dbname=library user=postgres password=301";

        // Устанавливаем соединение с базой данных
        pqxx::connection conn(conninfo);

        if (conn.is_open()) {
            std::cout << "Connected to database: " << conn.dbname() << std::endl;
            //Book b(1, "b1", 1,1,"g1");
            //Author a(1, "name",0);
            //a.save(conn);
            //b.save(conn);
            //Book b(conn, 1);
            //Author a(conn,1);
            //User u(1,"u1","2025-12-10");
            //u.save(conn);
            //User u(conn,1);
            Borrowed_book bb(1,1,"2025-12-10", "NULL");
            bb.save(conn);

        } else {
            std::cerr << "Failed to connect to the database." << std::endl;
            return 1;
        }

        // Закрываем соединение
        conn.close();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

#include <iostream>
#include <pqxx/pqxx> // Подключение библиотеки

int main() {
    try {
        // Строка подключения к базе данных
        std::string conninfo = "host=localhost port=5432 dbname=postgres user=postgres password=";

        // Устанавливаем соединение с базой данных
        pqxx::connection conn(conninfo);

        if (conn.is_open()) {
            std::cout << "Connected to database: " << conn.dbname() << std::endl;
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
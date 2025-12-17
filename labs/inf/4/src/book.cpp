#include <book.hpp>
void Book::save(pqxx::connection &conn, std::ostream &outp)
{
    try
    {
        // Начинаем транзакцию
        pqxx::work txn(conn);

        // SQL запрос с параметрами для предотвращения SQL-инъекций
        std::string sql =
            "INSERT INTO books (id, title, author_id, publication_year, genre) "
            "VALUES (" +
            std::to_string(id) + ", " +                    // id
            txn.quote(title) + ", " +                     // title (экранируется!)
            std::to_string(author_id) + ", " +            // author_id
            std::to_string(publication_year) + ", " +     // publication_year
            txn.quote(genre) +                            // genre (экранируется!)
            ")";

        //std::cout << "SQL запрос:" << std::endl;
        //std::cout << sql << std::endl;

        // Выполняем запрос
        txn.exec(sql);

        // Подтверждаем транзакцию
        txn.commit();

        outp << "Книга "<<title<<"("<<id<<") успешно сохранена" << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        throw; // Пробрасываем исключение дальше
    }
}
Book::Book(const pqxx::result &result, int index)
{
    try
    {
        id = result[index]["id"].as<int>();
        title = result[index]["title"].as<std::string>();
        author_id = result[index]["author_id"].as<int>();
        publication_year = result[index]["publication_year"].as<int>();
        genre = result[index]["genre"].as<std::string>();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        throw;
    }
}

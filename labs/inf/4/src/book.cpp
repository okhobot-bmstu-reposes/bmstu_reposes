#include <book.hpp>
void Book::save(pqxx::connection &conn)
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

        std::cout << "SQL запрос:" << std::endl;
        std::cout << sql << std::endl;

        // Выполняем запрос
        txn.exec(sql);

        // Подтверждаем транзакцию
        txn.commit();

        std::cout << "Книга успешно сохранена" << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Ошибка при сохранении книги: " << e.what() << std::endl;
        throw; // Пробрасываем исключение дальше
    }
}
Book Book::load(pqxx::connection &conn, int book_id)
{
    try
    {
        // Начинаем транзакцию
        pqxx::work txn(conn);

        // SQL запрос для получения данных книги
        std::string sql = "SELECT id, title, author_id, publication_year, genre "
                          "FROM books WHERE id = "+std::to_string(book_id);

        // Выполняем запрос
        pqxx::result result = txn.exec(sql);

        if (result.empty())
        {
            throw std::runtime_error("Книга с ID " + std::to_string(book_id) + " не найдена");
        }

        // Извлекаем данные из результата
        int id = result[0]["id"].as<int>();
        std::string title = result[0]["title"].as<std::string>();
        int author_id = result[0]["author_id"].as<int>();
        int publication_year = result[0]["publication_year"].as<int>();
        std::string genre = result[0]["genre"].as<std::string>();

        // Подтверждаем транзакцию
        txn.commit();

        // Создаем и возвращаем объект Book
        return Book(id, author_id, publication_year, title, genre);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Ошибка при загрузке книги: " << e.what() << std::endl;
        throw;
    }
}

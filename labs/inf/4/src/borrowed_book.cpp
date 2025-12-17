#include <borrowed_book.hpp>
void Borrowed_book::save(pqxx::connection &conn, std::ostream &outp)
{
    try
    {
        // Начинаем транзакцию
        pqxx::work txn(conn);

        // SQL запрос с параметрами для предотвращения SQL-инъекций
        std::string sql =
            "INSERT INTO borrowed_books (user_id, book_id, borrow_date, return_date) "
            "VALUES (" +
            std::to_string(user_id) + ", " +
            std::to_string(book_id) + ", " +
            txn.quote(borrow_date) + ", " +
            (return_date=="NULL"?return_date:txn.quote(return_date)) +
            ")";

        //std::cout << "SQL запрос:" << std::endl;
        //std::cout << sql << std::endl;

        // Выполняем запрос
        txn.exec(sql);

        // Подтверждаем транзакцию
        txn.commit();

        outp << "Книга с id="<<book_id<<" успешно взята" << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        throw; // Пробрасываем исключение дальше
    }
}
Borrowed_book::Borrowed_book(const pqxx::result &result, int index)
{
    try
    {
        user_id = result[index]["user_id"].as<int>();
        book_id = result[index]["book_id"].as<int>();
        borrow_date = result[index]["borrow_date"].as<std::string>();
        return_date = result[index]["return_date"].as<std::string>();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        throw;
    }
}

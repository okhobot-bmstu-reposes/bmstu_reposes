#include <iostream>
#include <memory>
#include <pqxx/pqxx>
class Borrowed_book
{
public:
    Borrowed_book(int user_id, int book_id, std::string borrow_date, std::string return_date)
        : user_id(user_id), book_id(book_id), borrow_date(borrow_date), return_date(return_date) {}
    Borrowed_book(const pqxx::result &result, int index=0);
    void save(pqxx::connection &conn);

private:
    std::string borrow_date, return_date;
    int user_id, book_id;
};

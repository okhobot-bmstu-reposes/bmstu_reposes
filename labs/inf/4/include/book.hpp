#include <iostream>
#include <memory>
#include <pqxx/pqxx>
class Book
{
public:
    Book(int id, std::string title, int author_id, int publication_year, std::string genre)
        : id(id), title(title), author_id(author_id), publication_year(publication_year), genre(genre) {}
    Book(const pqxx::result &result, int index=0);
    void save(pqxx::connection &conn);

private:
    std::string title, genre;
    int id, author_id, publication_year;
};

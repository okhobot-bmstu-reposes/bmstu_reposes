#include <iostream>
#include <memory>
#include <pqxx/pqxx>
class Book
{
public:
    Book(int id, std::string title, int author_id, int publication_year, std::string genre)
        : id(id), title(title), author_id(author_id), publication_year(publication_year), genre(genre) {}
    Book(const pqxx::result &result, int index = 0);
    Book(int id):id(id)
    {
        std::cin>>title>>author_id>>publication_year>>genre;
    }
    void save(pqxx::connection &conn, std::ostream &outp);

private:
    std::string title, genre;
    int id, author_id, publication_year;
};

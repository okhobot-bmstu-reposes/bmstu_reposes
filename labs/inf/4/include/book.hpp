#include <iostream>
#include <memory>
#include <pqxx/pqxx>
class Book
{
public:
    Book(int a_id, int a_author_id, int a_publication_year, std::string a_title, std::string a_genre)
        : id(a_id), author_id(a_author_id), publication_year(a_publication_year),
          title(a_title), genre(a_genre) {}
    void save(pqxx::connection &conn);
    Book load(pqxx::connection& conn, int book_id);

private:
    std::string title, genre;
    int id, author_id, publication_year;
};